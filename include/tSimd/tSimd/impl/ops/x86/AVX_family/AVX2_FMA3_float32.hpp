#pragma once

#include <immintrin.h> // AVX

#include "../../dispatch.hpp"
#include "AVX2_float32.hpp"

TSIMD_NAMESPACE_BEGIN

// AVX2 + FMA指令特化
template<>
struct SimdOp<SimdInstruction::AVX2_FMA3, float> : SimdOp<SimdInstruction::AVX2, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(AVX2_FMA3, float, __m256, Alignment::AVX_Family)

    TSIMD_OP_SIG_AVX2_FMA3(batch_t, mul_add, (batch_t a, batch_t b, batch_t c))
    {
        return _mm256_fmadd_ps(a, b, c);
    }
};
TSIMD_DETAIL_CHECK_SIMD_OP(SimdOp<SimdInstruction::AVX2_FMA3, float>)

TSIMD_NAMESPACE_END
