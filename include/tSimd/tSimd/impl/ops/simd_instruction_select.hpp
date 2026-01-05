#pragma once

#include <cstdint>
#include <type_traits>

#if defined(TSIMD_IS_TESTING)
    #include <cstdlib> // std::abort
#endif

#include "../platform.hpp"

#if defined(_MSC_VER)
    #include <intrin.h>
#else
    #include <cpuid.h>
#endif

TSIMD_NAMESPACE_BEGIN

namespace detail
{
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
        // xgetbv 指令编码为 0F 01 D0
        __asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
        return (static_cast<uint64_t>(edx) << 32) | eax;
#endif
    }

    // 首先需要读取 EAX 1 寄存器，接下来才能用下面的枚举判断
    enum class CpuFeatureIndex_EAX1 : uint32_t
    {
        // see in https://en.wikipedia.org/wiki/CPUID

        // ECX 寄存器的 feature
        XSAVE       = (1 << 26), // EAX 1, ECX 26
        OS_XSAVE    = (1 << 27), // EAX 1, ECX 27
        AVX         = (1 << 28), // EAX 1, ECX 28

        // EDX 寄存器的 feature
        SSE         = (1 << 25), // EAX 1, EDX 25
        SSE2        = (1 << 26), // EAX 1, EDX 26
    };

    template<typename E>
        requires std::is_enum_v<E>
    static constexpr uint32_t to_u32(E index) noexcept
    {
        return static_cast<uint32_t>(index);
    }

    // 检测高级指令集是否能使用，要顺便检测低级指令集以及其他附加的功能是否同时支持
    namespace CpuFeatureGroup_EDX
    {
        static constexpr uint32_t SSE = to_u32(CpuFeatureIndex_EAX1::SSE);
        static constexpr uint32_t SSE2 = to_u32(CpuFeatureIndex_EAX1::SSE2) | to_u32(CpuFeatureIndex_EAX1::SSE);
    }

    namespace CpuFeatureGroup_ECX
    {
        static constexpr uint32_t AVX = to_u32(CpuFeatureIndex_EAX1::AVX) | to_u32(CpuFeatureIndex_EAX1::OS_XSAVE) | to_u32(CpuFeatureIndex_EAX1::XSAVE);
    }

    inline bool cpu_feature_group_supports(uint32_t reg, uint32_t group_mask) noexcept
    {
        return ( (reg & group_mask) == group_mask );
    }
} // namespace detail


struct InstructionSetSupports
{
    bool AVX    = false;
    bool SSE2   = false;
};

inline InstructionSetSupports check_instruction_supports()
{
    InstructionSetSupports result{};

    uint32_t abcd[4]; // eax, ebx, ecx, edx

    detail::cpuid(0, 0, abcd);
    uint32_t max_leaf = abcd[0];
    if (max_leaf < 1)
    {
        return result;
    }

    // 查询 EAX 1
    detail::cpuid(1, 0, abcd);
    uint32_t ecx = abcd[2];
    uint32_t edx = abcd[3];

    result.SSE2 = detail::cpu_feature_group_supports(edx, detail::CpuFeatureGroup_EDX::SSE2);

    /* 检测 AVX 需要满足：
       1. CPUID.1:ECX.AVX[bit 28] == 1 (硬件支持 AVX)
       2. CPUID.1:ECX.OSXSAVE[bit 27] == 1 (OS 支持上下文切换保存寄存器)
       3. XCR0 寄存器中的 Bit 1 (SSE) 和 Bit 2 (AVX) 均为 1 (OS 已启用状态支持)
    */
    if (detail::cpu_feature_group_supports(ecx, detail::CpuFeatureGroup_ECX::AVX))
    {
        uint64_t xcr0 = detail::read_xcr0();
        // 0x6 表示 Bit 1 和 Bit 2 同时为 1
        if ((xcr0 & 0x6) == 0x6)
        {
            result.AVX = true;
        }
    }

    return result;
}


// 这个枚举的值就是函数指针表的索引，所以需要进行平台判断
enum class SimdInstruction : int
{
    Scalar = 0,

    // x86 指令集
#if defined(TSIMD_X86_ANY)
    SSE2,
    AVX,
#endif

    Num
};

class InstructionSelector final
{
private:
    static inline const InstructionSetSupports& get_support_info_impl() noexcept
    {
        static InstructionSetSupports result{};

        uint32_t abcd[4]; // eax, ebx, ecx, edx

        detail::cpuid(0, 0, abcd);
        uint32_t max_leaf = abcd[0];
        if (max_leaf < 1)
        {
            return result;
        }

        // 查询 EAX 1
        detail::cpuid(1, 0, abcd);
        uint32_t ecx = abcd[2];
        uint32_t edx = abcd[3];

        result.SSE2 = detail::cpu_feature_group_supports(edx, detail::CpuFeatureGroup_EDX::SSE2);

        /* 检测 AVX 需要满足：
           1. CPUID.1:ECX.AVX[bit 28] == 1 (硬件支持 AVX)
           2. CPUID.1:ECX.OSXSAVE[bit 27] == 1 (OS 支持上下文切换保存寄存器)
           3. XCR0 寄存器中的 Bit 1 (SSE) 和 Bit 2 (AVX) 均为 1 (OS 已启用状态支持)
        */
        if (detail::cpu_feature_group_supports(ecx, detail::CpuFeatureGroup_ECX::AVX))
        {
            uint64_t xcr0 = detail::read_xcr0();
            // 0x6 表示 Bit 1 和 Bit 2 同时为 1
            if ((xcr0 & 0x6) == 0x6)
            {
                result.AVX = true;
            }
        }

        return result;
    }

public:
    static inline const InstructionSetSupports& get_support_info() noexcept
    {
        static const InstructionSetSupports& s = get_support_info_impl();
        return s;
    }

private:
    static inline int select_func_index()
    {
        const auto& supports = get_support_info();
        
        // 如果正在测试，则强制选择那个指令，如果那个指令不支持，则直接报错退出即可
#if defined(TSIMD_TEST_INTRINSIC)
        int fn_idx = static_cast<int>(SimdInstruction::TSIMD_TEST_INTRINSIC);
        if (!supports.TSIMD_TEST_INTRINSIC)
        {
            std::abort();
        }
        return fn_idx;
#endif

        // 从最高级的指令往下判断
        if (supports.AVX)
        {
            return static_cast<int>(SimdInstruction::AVX);
        }

        if (supports.SSE2)
        {
            return static_cast<int>(SimdInstruction::SSE2);
        }
        
        // fallback to scalar
        return static_cast<int>(SimdInstruction::Scalar);
    }
    
    static inline size_t compute_alignment() noexcept
    {
        const auto& supports = get_support_info();
        if (supports.AVX)
        {
            return 32;
        }
        if (supports.SSE2)
        {
            return 16;
        }

        // scalar = 4
        return 4;
    }

public:
    static inline int dyn_func_index() noexcept
    {
        static int idx = select_func_index();
        return idx;
    }
    
    static inline size_t required_alignment() noexcept
    {
        static size_t a = compute_alignment();
        return a;
    }
};


// --------------------------------- SimdOp 宏 ---------------------------------
#define TSIMD_CURRENT_OP(scalar_type) \
    TSIMD_NAMESPACE_NAME::TSIMD_DYN_INSTRUCTION::SimdOp::scalar_type##_ns

#define TSIMD_DETAIL_SCALAR_ELEM_TYPE_NS(type) type##_ns

#define TSIMD_DETAIL_BEGIN_DECL_SIMD_OP(instruction, scalar_elem_type, batch_type) \
    namespace TSIMD_NAMESPACE_NAME::TSIMD_DETAIL_DYN_DISPATCH_NS_PREFIX::instruction::SimdOp \
    { \
        struct TSIMD_DETAIL_SCALAR_ELEM_TYPE_NS(scalar_elem_type) \
        { \
            TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(instruction, scalar_elem_type, batch_type)

#define TSIMD_DETAIL_END_DECL_SIMD_OP() \
        }; \
    }

#define TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(instruction_type, elem_type, batch_type) \
    /* 类型萃取 */ \
    using batch_t = batch_type; \
    \
    /* 常量 */ \
    static constexpr TSIMD_NAMESPACE_NAME::SimdInstruction CurrentInstruction = TSIMD_NAMESPACE_NAME::SimdInstruction::instruction_type; \
    static constexpr size_t BatchSize = sizeof(batch_t); \
    static constexpr size_t ElementSize = sizeof(elem_type); \
    static constexpr size_t Lanes = (BatchSize / ElementSize); \
    static constexpr size_t BatchAlignment = alignof(batch_type); \
    \
    /* static check */ \
    static_assert((Lanes % 4 == 0) || (Lanes == 1 && std::is_arithmetic_v<elem_type>), "Lanes must be 4 * N or 1(for scalar type)");



TSIMD_NAMESPACE_END
