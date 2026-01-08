#pragma once

#include <cstdint>

#if defined(TSIMD_TEST_INTRINSIC) && defined(TSIMD_IS_TESTING)
    #include <cstdlib> // std::abort
#endif

#if defined(_MSC_VER)
    #include <intrin.h>
#else
    #include <cpuid.h>
#endif

#include <type_traits>
#include <concepts>
#include <utility>

#include "../platform.hpp"
#include "func_attr.hpp"

TSIMD_NAMESPACE_BEGIN

namespace detail
{
    // leaf: EAX, sub_leaf: ECX
    inline void cpuid(const uint32_t leaf, const uint32_t sub_leaf, uint32_t* abcd)
    {
#if defined(_MSC_VER)
        int regs[4];
        __cpuidex(regs, static_cast<int>(leaf), static_cast<int>(sub_leaf));
        for (int i = 0; i < 4; ++i)
        {
            abcd[i] = static_cast<uint32_t>(regs[i]);
        }
#else
        uint32_t a;
        uint32_t b;
        uint32_t c;
        uint32_t d;
        __cpuid_count(leaf, sub_leaf, a, b, c, d);
        abcd[0] = a;
        abcd[1] = b;
        abcd[2] = c;
        abcd[3] = d;
#endif
    }

    inline uint64_t read_xcr0()
    {
#if defined(_MSC_VER)
        return _xgetbv(0);
#else
        uint32_t eax, edx;
        __asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
        return (static_cast<uint64_t>(edx) << 32) | eax;
#endif
    }

    // 首先需要读取 EAX 1 寄存器，接下来才能用下面的枚举判断
    enum class CpuFeatureIndex_EAX1 : uint32_t
    {
        // see https://en.wikipedia.org/wiki/CPUID

        // ECX 寄存器的 feature
        SSE3        = 0 , // EAX 1, ECX  0
        SSSE3       = 9 , // EAX 1, ECX  9
        FMA3        = 12, // EAX 1, ECX 12
        SSE4_1      = 19, // EAX 1, ECX 19
        SSE4_2      = 20, // EAX 1, ECX 20
        XSAVE       = 26, // EAX 1, ECX 26
        OS_XSAVE    = 27, // EAX 1, ECX 27
        AVX         = 28, // EAX 1, ECX 28
        F16C        = 29, // EAX 1, ECX 29

        // EDX 寄存器的 feature
        SSE         = 25, // EAX 1, EDX 25
        SSE2        = 26, // EAX 1, EDX 26
    };

    enum class CpuFeatureIndex_EAX7 : uint32_t
    {
        AVX2        = 5 , // EAX 7, EBX  5
        AVX_512_F   = 16, // EAX 7, EBX 16
    };

    enum class CpuXSaveStateIndex : uint64_t
    {
        // see https://en.wikipedia.org/wiki/CPUID XSAVE State-components

        // bit 1: SSE state: XMM0-XMM15 and MXCSR
        // bit 2: AVX: YMM0-YMM15
        // bit 5: AVX-512: opmask registers k0-k7
        // bit 6: AVX-512: ZMM_Hi256 ZMM0-ZMM15
        // bit 7: AVX-512: Hi16_ZMM ZMM16-ZMM31

        SSE                 = 1 , // XMM0-XMM15 and MXCSR
        AVX                 = 2 , // YMM0-YMM15
        AVX_512_K0_K7       = 5 , // opmask registers k0-k7
        AVX_512_LOW_256     = 6 , // ZMM0-ZMM15
        AVX_512_HIGH_256    = 7 , // ZMM16-ZMM31
    };

    template<typename T>
    using underlying_t =
        std::conditional_t<
            std::is_enum_v<T>,
            std::underlying_type_t<T>,
            T
        >;

    template<typename T>
        requires (std::is_enum_v<T> || std::is_integral_v<T>)
    static constexpr underlying_t<T> underlying(const T val) noexcept
    {
        return static_cast<underlying_t<T>>(val);
    }

    template<typename T, typename U>
    static constexpr bool bit_is_open(T data, U bit_pos) noexcept
    {
        static_assert(sizeof(T) == sizeof(U));

        using Type = underlying_t<U>;
        return (static_cast<Type>(data) & (static_cast<Type>(1) << static_cast<Type>(bit_pos))) != 0;
    }
} // namespace detail


struct InstructionSetSupports
{
    // 从低到高排序

    static constexpr bool Scalar = true;

    // SSE family
    bool SSE        = false;
    bool SSE2       = false;
    bool SSE3       = false;
    bool SSSE3      = false;
    bool SSE4_1     = false;
    bool SSE4_2     = false;

    // AVX family
    bool AVX        = false;

    // 这两个是独立指令集，在tsimd库中，AVX的op不使用FMA3指令，AVX2的op分成两套:
    // 1. AVX2, 2. AVX2+FMA3。一套不使用FMA3，另一套使用FMA3
    bool F16C = false, FMA3 = false;

    bool AVX2       = false;
    bool AVX2_FMA3  = false;

    // AVX-512 family
    bool AVX512_F   = false; // AVX512F支持FMA运算，不需要单独划分FMA3支持
};

namespace detail
{
    // 这个枚举的值就是函数指针表的索引
    enum class SimdInstructionIndex : int
    {
        Invalid = -1,

    #if defined(TSIMD_INSTRUCTION_FEATURE_SCALAR)
        Scalar,
    #endif

    #if defined(TSIMD_INSTRUCTION_FEATURE_SSE)
        SSE,
    #endif

    #if defined(TSIMD_INSTRUCTION_FEATURE_SSE2)
        SSE2,
    #endif

    #if defined(TSIMD_INSTRUCTION_FEATURE_AVX)
        AVX,
    #endif

    #if defined(TSIMD_INSTRUCTION_FEATURE_AVX2)
        AVX2,
    #endif

    #if defined(TSIMD_INSTRUCTION_FEATURE_AVX2) && defined(TSIMD_INSTRUCTION_FEATURE_FMA3)
        AVX2_FMA3,
    #endif

        Num
    };
    static_assert(underlying(SimdInstructionIndex::Num) > 0);
}

class InstructionSelector final
{
public:
    static const InstructionSetSupports& get_support_info() noexcept;
    static size_t required_alignment() noexcept;

    // 测试时直接返回索引即可，正式版本才使用运行时CPUID判断
    static int dyn_func_index() noexcept;
};


// instruction充当命名空间
#define TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, instruction) \
    &TSIMD_NAMESPACE_NAME::instruction::func_name,

#define TSIMD_DETAIL_ONE_EMPTY_FUNC

// ---------------------------------------------- Function table ----------------------------------------------
// Scalar
#if defined(TSIMD_INSTRUCTION_FEATURE_SCALAR)
    #define TSIMD_DETAIL_SCALAR_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, Scalar)
#else
    #define TSIMD_DETAIL_SCALAR_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// SSE
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE)
    #define TSIMD_DETAIL_SSE_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, SSE)
#else
    #define TSIMD_DETAIL_SSE_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// SSE2
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE2)
    #define TSIMD_DETAIL_SSE2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, SSE2)
#else
    #define TSIMD_DETAIL_SSE2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// AVX
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX)
    #define TSIMD_DETAIL_AVX_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, AVX)
#else
    #define TSIMD_DETAIL_AVX_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// AVX2
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX2)
    #define TSIMD_DETAIL_AVX2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, AVX2)
#else
    #define TSIMD_DETAIL_AVX2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// AVX_FMA3
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX2) && defined(TSIMD_INSTRUCTION_FEATURE_FMA3)
    #define TSIMD_DETAIL_AVX2_FMA3_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, AVX2_FMA3)
#else
    #define TSIMD_DETAIL_AVX2_FMA3_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// function table
#define TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY(func_name) \
    /* ------------------------------------- scalar ------------------------------------- */ \
    TSIMD_DETAIL_SCALAR_FUNC_IMPL(func_name) \
    /* ------------------------------------- x86 ------------------------------------- */ \
    TSIMD_DETAIL_SSE_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_SSE2_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_AVX_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_AVX2_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_AVX2_FMA3_FUNC_IMPL(func_name)

#if !defined(TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY)
    #error "have not defined DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY to cache the simd function pointers"
#endif

#define TSIMD_DYN_DISPATCH_FUNC(func_name) \
    /* 构建静态数组，存储函数指针 */ \
    static inline decltype(&TSIMD_NAMESPACE_NAME::TSIMD_DYN_INSTRUCTION::func_name) TSIMD_PFN_##func_name##_table[] = { \
        TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY(func_name) \
    };

// 测试时直接返回索引即可，正式版本才使用运行时CPUID判断
#if defined(TSIMD_TEST_INTRINSIC) && defined(TSIMD_IS_TESTING)
    #define TSIMD_DYN_FUNC_POINTER(func_name) \
        []() { \
            /* 用一个lambda表达式获取索引并判断索引是否可用，如果不可用，直接 abort */ \
            int idx = static_cast<int>(TSIMD_NAMESPACE_NAME::detail::SimdInstructionIndex::TSIMD_TEST_INTRINSIC); \
            const auto& supports = TSIMD_NAMESPACE_NAME::InstructionSelector::get_support_info(); \
            if (!supports.TSIMD_TEST_INTRINSIC) { std::abort(); } \
            return TSIMD_PFN_##func_name##_table[idx]; \
        }()
#else
    #define TSIMD_DYN_FUNC_POINTER(func_name) \
        TSIMD_PFN_##func_name##_table[TSIMD_NAMESPACE_NAME::InstructionSelector::dyn_func_index()]
#endif


#define TSIMD_DYN_CALL(func_name) (TSIMD_DYN_FUNC_POINTER(func_name))



// --------------------------------- FUNC_ATTR字符串描述 ---------------------------------
// 将会在 dispatch_this_file.hpp 文件被多次重定义
#define TSIMD_DYN_FUNC_ATTR tsimd_error_attr



// --------------------------------- SimdOp ---------------------------------
// 这个枚举用于SimdOp的模板参数
enum class SimdInstruction : int
{
    Scalar,

    SSE,
    SSE2,
    AVX,
    AVX2,
    AVX2_FMA3,

    Num
};

template<typename T>
concept scalar_type = std::is_same_v<T, float>;

template<SimdInstruction Instruction, scalar_type ScalarType>
struct SimdOp
{
    static_assert(Instruction >= SimdInstruction::Num || Instruction < SimdInstruction::Num);
};

#define TSIMD_DYN_SIMD_OP(scalar_type) \
    SimdOp<SimdInstruction::TSIMD_DYN_INSTRUCTION, scalar_type>

#define TSIMD_DETAIL_SIMD_OP_STRUCT_NAME(instruction, scalar_elem_type) \
    SimdOp_##instruction##_##scalar_elem_type

#define TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(instruction_type, elem_type, batch_type, batch_alignment) \
    /* 类型萃取 */ \
    using batch_t = batch_type; \
    using scalar_t = elem_type; \
    \
    /* 常量 */ \
    static constexpr SimdInstruction CurrentInstruction = SimdInstruction::instruction_type; \
    static constexpr size_t BatchSize = sizeof(batch_t); \
    static constexpr size_t ElementSize = sizeof(elem_type); \
    static constexpr size_t Lanes = (BatchSize / ElementSize); \
    static constexpr size_t BatchAlignment = batch_alignment; \
    \
    /* static check */ \
    static_assert(Lanes % 2 == 0 || Lanes == 1, "Lanes must be 2 * N or 1");

namespace detail
{
    template<typename T>
    static consteval bool check_simd_op()
    {
        if constexpr (std::has_virtual_destructor_v<T>)
        {
            return false;
        }
        return true;
    }
}
#define TSIMD_DETAIL_CHECK_SIMD_OP(...) static_assert(detail::check_simd_op<__VA_ARGS__>(), "SimdOp static check failed.");


TSIMD_NAMESPACE_END
