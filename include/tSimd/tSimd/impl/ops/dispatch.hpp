#pragma once

#include <cstdint>

#if defined(TSIMD_TEST_INTRINSIC) && defined(TSIMD_IS_TESTING)
    #include <cstdlib> // std::abort
#endif

#include <type_traits>
#include <concepts>

#include "../platform.hpp"
#include "func_attr.hpp"

TSIMD_NAMESPACE_BEGIN

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

// --------------------------------- TSIMD_DYN_INSTRUCTION str ---------------------------------
#define TSIMD_DYN_INSTRUCTION_SCALAR     Scalar
#define TSIMD_DYN_INSTRUCTION_SSE        SSE
#define TSIMD_DYN_INSTRUCTION_SSE2       SSE2
#define TSIMD_DYN_INSTRUCTION_SSE3       SSE3
#define TSIMD_DYN_INSTRUCTION_SSSE3      SSSE3
#define TSIMD_DYN_INSTRUCTION_SSE4_1     SSE4_1
#define TSIMD_DYN_INSTRUCTION_SSE4_2     SSE4_2
#define TSIMD_DYN_INSTRUCTION_AVX        AVX
#define TSIMD_DYN_INSTRUCTION_AVX2       AVX2
#define TSIMD_DYN_INSTRUCTION_AVX2_FMA3  AVX2_FMA3

namespace detail
{
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

    #if defined(TSIMD_INSTRUCTION_FEATURE_SSE3)
        SSE3,
    #endif

    #if defined(TSIMD_INSTRUCTION_FEATURE_SSE4_1)
        SSE4_1,
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

// instruction充当命名空间
#define TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, instruction) \
    &TSIMD_NAMESPACE_NAME::instruction::func_name,

#define TSIMD_DETAIL_ONE_EMPTY_FUNC

// ---------------------------------------------- Function table ----------------------------------------------
// Scalar
#if defined(TSIMD_INSTRUCTION_FEATURE_SCALAR)
    #define TSIMD_DETAIL_SCALAR_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, TSIMD_DYN_INSTRUCTION_SCALAR)
#else
    #define TSIMD_DETAIL_SCALAR_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// SSE
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE)
    #define TSIMD_DETAIL_SSE_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, TSIMD_DYN_INSTRUCTION_SSE)
#else
    #define TSIMD_DETAIL_SSE_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// SSE2
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE2)
    #define TSIMD_DETAIL_SSE2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, TSIMD_DYN_INSTRUCTION_SSE2)
#else
    #define TSIMD_DETAIL_SSE2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// SSE3
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE3)
    #define TSIMD_DETAIL_SSE3_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, TSIMD_DYN_INSTRUCTION_SSE3)
#else
    #define TSIMD_DETAIL_SSE3_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// SSE4.1
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE4_1)
    #define TSIMD_DETAIL_SSE4_1_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, TSIMD_DYN_INSTRUCTION_SSE4_1)
#else
    #define TSIMD_DETAIL_SSE4_1_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// AVX
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX)
    #define TSIMD_DETAIL_AVX_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, TSIMD_DYN_INSTRUCTION_AVX)
#else
    #define TSIMD_DETAIL_AVX_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// AVX2
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX2)
    #define TSIMD_DETAIL_AVX2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, TSIMD_DYN_INSTRUCTION_AVX2)
#else
    #define TSIMD_DETAIL_AVX2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_EMPTY_FUNC
#endif

// AVX_FMA3
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX2) && defined(TSIMD_INSTRUCTION_FEATURE_FMA3)
    #define TSIMD_DETAIL_AVX2_FMA3_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, TSIMD_DYN_INSTRUCTION_AVX2_FMA3)
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
    TSIMD_DETAIL_SSE3_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_SSE4_1_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_AVX_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_AVX2_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_AVX2_FMA3_FUNC_IMPL(func_name)

#if !defined(TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY)
    #error "have not defined DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY to cache the simd function pointers"
#endif


// -------------------------- dispatch function ---------------------------
class InstructionSelector final
{
public:
    static const InstructionSetSupports& get_support_info() noexcept;
    static size_t required_alignment() noexcept;

    // 测试时直接返回索引即可，正式版本才使用运行时CPUID判断
    static int dyn_func_index() noexcept;
};

#define TSIMD_DYN_DISPATCH_FUNC(func_name) \
    /* 构建静态数组，存储函数指针 (使用命名空间包裹，限定只能在类外使用) */ \
    namespace TSIMD_NAMESPACE_NAME::PFN_table { \
        static inline decltype(&TSIMD_NAMESPACE_NAME::TSIMD_DYN_INSTRUCTION::func_name) func_name[] = { \
            TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY(func_name) \
        }; \
    }

// 测试时直接返回索引即可，正式版本才使用运行时CPUID判断
#if defined(TSIMD_TEST_INTRINSIC) && defined(TSIMD_IS_TESTING)
    #define TSIMD_DYN_FUNC_POINTER(func_name) \
        []() { \
            /* 用一个lambda表达式获取索引并判断索引是否可用，如果不可用，直接 abort */ \
            int idx = static_cast<int>(TSIMD_NAMESPACE_NAME::detail::SimdInstructionIndex::TSIMD_TEST_INTRINSIC); \
            const auto& supports = TSIMD_NAMESPACE_NAME::InstructionSelector::get_support_info(); \
            if (!supports.TSIMD_TEST_INTRINSIC) { std::abort(); } \
            return TSIMD_NAMESPACE_NAME::PFN_table::func_name[idx]; \
        }()
#else
    #define TSIMD_DYN_FUNC_POINTER(func_name) \
        TSIMD_NAMESPACE_NAME::PFN_table::func_name[TSIMD_NAMESPACE_NAME::InstructionSelector::dyn_func_index()]
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
    SSE3,
    SSE4_1,
    AVX,
    AVX2,
    AVX2_FMA3
};

template<typename T>
concept scalar_type = std::is_same_v<T, float>;

template<SimdInstruction Instruction, scalar_type ScalarType>
struct SimdOp;

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
        using scalar_t = T::scalar_t;

        // 不支持 long double
        if constexpr (std::is_same_v<long double, scalar_t>)
        {
            return false;
        }

        // 不能有虚函数
        if constexpr (std::has_virtual_destructor_v<T>)
        {
            return false;
        }

        return true;
    }
}
#define TSIMD_DETAIL_CHECK_SIMD_OP(...) static_assert(detail::check_simd_op<__VA_ARGS__>(), "SimdOp static check failed.")
#define TSIMD_DETAIL_CHECK_SCALAR_OP(...) static_assert(detail::check_simd_op<__VA_ARGS__>(), "Invalid scalar op.")

TSIMD_NAMESPACE_END
