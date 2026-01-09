#include "tSimd/impl/ops/dispatch.hpp"

#include <utility>

TSIMD_NAMESPACE_BEGIN

namespace detail
{
    const InstructionSetSupports& get_support_info_impl() noexcept
    {
        using namespace detail;

        static InstructionSetSupports result{};

#if defined(TSIMD_ARCH_X86_ANY)
        uint32_t abcd[4]; // eax, ebx, ecx, edx

        cpuid(0, 0, abcd);
        const uint32_t max_leaf = abcd[0];


        // ------------------ EAX 1 ------------------
        if (max_leaf < 1) // 因为要读取EAX 1，所以 max leaf 必须 >= 1
        {
            return result;
        }

        // 查询 EAX 1, ECX 0
        cpuid(1, 0, abcd);
        const uint32_t ecx = abcd[2];
        const uint32_t edx = abcd[3];

        // ------------------------- SSE family -------------------------
        result.SSE = bit_is_open(edx, CpuFeatureIndex_EAX1::SSE);
        result.SSE2 = result.SSE && bit_is_open(edx, CpuFeatureIndex_EAX1::SSE2);
        result.SSE3 = result.SSE2 && bit_is_open(ecx, CpuFeatureIndex_EAX1::SSE3);
        result.SSSE3 = result.SSE3 && bit_is_open(ecx, CpuFeatureIndex_EAX1::SSSE3);
        result.SSE4_1 = result.SSSE3 && bit_is_open(ecx, CpuFeatureIndex_EAX1::SSE4_1);
        result.SSE4_2 = result.SSE4_1 && bit_is_open(ecx, CpuFeatureIndex_EAX1::SSE4_2);


        const bool xsave = bit_is_open(ecx, CpuFeatureIndex_EAX1::XSAVE);
        const bool os_xsave = bit_is_open(ecx, CpuFeatureIndex_EAX1::OS_XSAVE);
        // 只有在 xsave 和 os_xsave 为帧的时候，才能进行 xcr0 检查
        if (xsave && os_xsave)
        {
            const uint64_t xcr0 = read_xcr0();

            // ------------------------- AVX family -------------------------
            const bool os_support_avx = bit_is_open(xcr0, CpuXSaveStateIndex::SSE) &&
                                        bit_is_open(xcr0, CpuXSaveStateIndex::AVX);

            result.AVX = result.SSE3 && bit_is_open(ecx, CpuFeatureIndex_EAX1::AVX) && os_support_avx;
            result.F16C = result.AVX && bit_is_open(ecx, CpuFeatureIndex_EAX1::F16C);
            result.FMA3 = result.AVX && bit_is_open(ecx, CpuFeatureIndex_EAX1::FMA3);

            // ------------------ EAX 7 ------------------
            if (max_leaf < 7) // 因为要读取EAX 7，所以 max leaf 必须 >= 7
            {
                return result;
            }

            // EAX 7, ECX 0
            cpuid(7, 0, abcd);
            const uint32_t ebx = abcd[1];

            result.AVX2 = result.AVX && bit_is_open(ebx, CpuFeatureIndex_EAX7::AVX2);
            result.AVX2_FMA3 = result.AVX2 && result.FMA3;


            // ------------------------- AVX-512 family -------------------------
            const bool os_support_avx_512 = os_support_avx &&
                                            bit_is_open(xcr0, CpuXSaveStateIndex::AVX_512_K0_K7) &&
                                            bit_is_open(xcr0, CpuXSaveStateIndex::AVX_512_LOW_256) &&
                                            bit_is_open(xcr0, CpuXSaveStateIndex::AVX_512_HIGH_256);

            result.AVX512_F = result.AVX2 && bit_is_open(ebx, CpuFeatureIndex_EAX7::AVX_512_F) && os_support_avx_512;
        }

        return result;
#else
        // arm NEON
#endif
    }

    int dyn_func_index_impl() noexcept
    {
        const auto& supports = get_support_info_impl();

        // 从最高级的指令往下判断
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX2) && defined(TSIMD_INSTRUCTION_FEATURE_FMA3)
        if (supports.AVX2_FMA3)
        {
            return underlying(SimdInstructionIndex::AVX2_FMA3);
        }
#endif

#if defined(TSIMD_INSTRUCTION_FEATURE_AVX2)
        if (supports.AVX2)
        {
            return underlying(SimdInstructionIndex::AVX2);
        }
#endif

#if defined(TSIMD_INSTRUCTION_FEATURE_AVX)
        if (supports.AVX)
        {
            return underlying(SimdInstructionIndex::AVX);
        }
#endif

#if defined(TSIMD_INSTRUCTION_FEATURE_SSE2)
        if (supports.SSE2)
        {
            return underlying(SimdInstructionIndex::SSE2);
        }
#endif

#if defined(TSIMD_INSTRUCTION_FEATURE_SSE)
        if (supports.SSE)
        {
            return underlying(SimdInstructionIndex::SSE);
        }
#endif

#if defined(TSIMD_INSTRUCTION_FEATURE_SCALAR)
        return underlying(SimdInstructionIndex::Scalar);
#endif

        std::unreachable();
    }

    size_t required_alignment() noexcept
    {
        const auto& supports = get_support_info_impl();
        if (supports.AVX512_F)
        {
            return Alignment::AVX512_Family;
        }
        if (supports.AVX)
        {
            return Alignment::AVX_Family;
        }
        if (supports.SSE)
        {
            return Alignment::SSE_Family;
        }

        return Alignment::Scalar;
    }
}

// 测试时，这段代码不会被调用
int InstructionSelector::dyn_func_index() noexcept
{
    static int i = detail::dyn_func_index_impl();
    return i;
}

const InstructionSetSupports& InstructionSelector::get_support_info() noexcept
{
    static const InstructionSetSupports& s = detail::get_support_info_impl();
    return s;
}

size_t InstructionSelector::required_alignment() noexcept
{
    static size_t a = detail::required_alignment();
    return a;
}

TSIMD_NAMESPACE_END
