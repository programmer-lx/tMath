#pragma once

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

#if defined(TSIMD_USE_SSE2)
    result.SSE2 = detail::cpu_feature_group_supports(edx, detail::CpuFeatureGroup_EDX::SSE2);
#endif

#if defined(TSIMD_USE_AVX)
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
#endif

    return result;
}



enum class SimdInstruction : int
{
    Scalar = 0,

#if defined(TSIMD_USE_SSE2)
    SSE2,
#endif

#if defined(TSIMD_USE_AVX)
    AVX,
#endif

    Num
};

class InstructionSelector final
{
private:
    static inline bool m_init = false;
    static inline InstructionSetSupports m_supports{};
    static inline int m_func_index = -1;

private:
    static inline void select_func_index()
    {
        // 如果正在测试，则强制选择那个指令，如果那个指令不支持，则直接报错退出即可
#if defined(TSIMD_IS_TESTING)
    #if defined(TSIMD_TEST_AVX)
        m_func_index = static_cast<int>(SimdInstruction::AVX);
        if (!m_supports.AVX)
        {
            std::abort();
        }
        return;
    #endif
    #if defined(TSIMD_TEST_SSE2)
        m_func_index = static_cast<int>(SimdInstruction::SSE2);
        if (!m_supports.SSE2)
        {
            std::abort();
        }
        return;
    #endif
    #if defined(TSIMD_TEST_SCALAR)
        m_func_index = static_cast<int>(SimdInstruction::Scalar);
        return;
    #endif
#endif

        // 从最高级的指令往下判断
#if defined(TSIMD_USE_AVX)
        if (m_supports.AVX)
        {
            m_func_index = static_cast<int>(SimdInstruction::AVX);
            return;
        }
#endif

#if defined(TSIMD_USE_SSE2)
        if (m_supports.SSE2)
        {
            m_func_index = static_cast<int>(SimdInstruction::SSE2);
            return;
        }
#endif
        // fallback to scalar
        m_func_index = static_cast<int>(SimdInstruction::Scalar);
    }

public:
    static inline void init() noexcept
    {
        if (!m_init)
        {
            m_supports = check_instruction_supports();
            select_func_index();
        }

        m_init = true;
    }

    static inline const InstructionSetSupports& get_support_info() noexcept
    {
        return m_supports;
    }

    static inline int dyn_func_index() noexcept { return m_func_index; }
};

template<typename T>
concept scalar_elem_type = std::is_arithmetic_v<T>;

template<SimdInstruction op_instruction, scalar_elem_type Elem>
struct SimdOp
{
    static_assert(op_instruction != SimdInstruction::Num,
        "Please set a backend when using SimdOp<...>, for example: SimdOp<SimdInstruction::SSE2>, SimdOp<SimdInstruction::Scalar>");
};

#define TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(instruction_type, elem_type, batch_type) \
    /* 类型萃取 */ \
    using batch_t = batch_type; \
    \
    /* 常量 */ \
    static constexpr SimdInstruction CurrentInstruction = SimdInstruction::instruction_type; \
    static constexpr size_t BatchSize = sizeof(batch_t); \
    static constexpr size_t ElementSize = sizeof(elem_type); \
    static constexpr size_t Lanes = (BatchSize / ElementSize); \
    static constexpr size_t BatchAlignment = alignof(batch_type); \
    \
    /* static check */ \
    static_assert(Lanes % 4 == 0 || Lanes == 1, "Lanes must be 4 * N or 1");


#define TSIMD_DETAIL_INSTRUCTION_T TSIMD_NAMESPACE_NAME::SimdInstruction
#define TSIMD_DETAIL_CURRENT_TEMPLATE_INSTRUCTION_NAME CurrentSimdInstruction

#define TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, instruction) \
    func_name##_dispatch_impl<TSIMD_NAMESPACE_NAME::SimdInstruction::instruction>,

#define TSIMD_DETAIL_SCALAR_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, Scalar)

#if defined(TSIMD_USE_SSE2)
    #define TSIMD_DETAIL_SSE2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, SSE2)
#else
    #define TSIMD_DETAIL_SSE2_FUNC_IMPL(...)
#endif

#if defined(TSIMD_USE_AVX)
    #define TSIMD_DETAIL_AVX_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, AVX)
#else
    #define TSIMD_DETAIL_AVX_FUNC_IMPL(...)
#endif

// 不同后端的函数指针
// 这里需要仔细设计，
// 因为这里需要根据不同的编译宏开关，来调整这个数组的大小，以及指令集实现版本的存放位置
#define TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY(func_name) \
    TSIMD_DETAIL_SCALAR_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_SSE2_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_AVX_FUNC_IMPL(func_name)

#if !defined(TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY)
    #error "have not defined DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY to cache the simd function pointers"
#endif

/**
 * @brief 辅助宏，用于直接声明并定义动态分发函数\n
 * 括号内写完整的函数签名，比如: , void, , kernel_func, (float* data, size_t size, float& out_sum), noexcept\n
 * 格式: 前缀, 返回值类型, 调用约定, call conv, 函数名, (参数签名), 尾随修饰符\n
 * 然后在宏之后，紧跟 {} 编写函数定义\n
 * 调用函数时，先需要获取函数指针，再调用，example: kernel_func()(arr.data(), arr.size(), sum);
 *
 * @param prefix 返回值前面的修饰符，比如 [[nodiscard]] 之类的，但是要注意，不能添加 inline，因为 inline 在宏内已经添加了
 * @param ret 返回值类型
 * @param call_conv 调用约定，比如 __cdecl, __vectorcall
 * @param func_name 函数名
 * @param params 函数参数，带括号
 * @param specifiers 函数尾随修饰符，比如 noexcept
 */
#define TSIMD_DYN_DISPATCH_FUNC(prefix, ret, call_conv, func_name, params, specifiers) \
    /* 声明 impl 模板 */ \
    template<TSIMD_DETAIL_INSTRUCTION_T TSIMD_DETAIL_CURRENT_TEMPLATE_INSTRUCTION_NAME> \
    prefix static inline ret call_conv func_name##_dispatch_impl params specifiers; \
    \
    /* 定义函数指针类型 */ \
    using PFN_##func_name##_dispatch_impl = ret (call_conv *) params specifiers; \
    \
    /* dispatch 表 (一个静态数组) */ \
    static inline const PFN_##func_name##_dispatch_impl PFN_##func_name##_dispatch_impl_table[] = { \
        TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY(func_name) \
    }; \
    \
    /* 入口函数 (不是直接调用，而是获取实际分发的函数指针) */ \
    static inline PFN_##func_name##_dispatch_impl call_conv func_name() noexcept { \
        static auto fn = PFN_##func_name##_dispatch_impl_table[TSIMD_NAMESPACE_NAME::InstructionSelector::dyn_func_index()]; \
        return fn; \
    } \
    \
    template<TSIMD_DETAIL_INSTRUCTION_T TSIMD_DETAIL_CURRENT_TEMPLATE_INSTRUCTION_NAME> \
    prefix inline ret call_conv func_name##_dispatch_impl params specifiers


TSIMD_NAMESPACE_END
