#pragma once

#include "AVX2_float32.hpp"

TSIMD_NAMESPACE_BEGIN

// AVX2 + FMA指令特化
template<>
struct SimdOp<SimdInstruction::AVX2_FMA3, float32> : SimdOp<SimdInstruction::AVX2, float32>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(AVX2_FMA3, float32, AVX_family::Batch<float32>, Alignment::AVX_Family)

    TSIMD_OP_SIG_AVX2_FMA3(batch_t, mul_add, (batch_t a, batch_t b, batch_t c))
    {
        return { _mm256_fmadd_ps(a.v, b.v, c.v) };
    }
};
TSIMD_DETAIL_CHECK_SIMD_OP(SimdOp<SimdInstruction::AVX2_FMA3, float32>);

TSIMD_NAMESPACE_END
