#pragma once

#include <pmmintrin.h> // SSE3

#include "../../dispatch.hpp"
#include "SSE2_float32.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::SSE3, float> : SimdOp<SimdInstruction::SSE2, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(SSE3, float, __m128, Alignment::SSE_Family)

    TSIMD_OP_SIG_SSE3(float, reduce_sum, (batch_t v))
    {
        // input: [d, c, b, a]
        // hadd: [c+d, a+b, c+d, a+b]
        // hadd: [a+b+c+d, .........]
        // get lane[0]

        __m128 result = _mm_hadd_ps(v, v);
        result = _mm_hadd_ps(result, result);
        return _mm_cvtss_f32(result);
    }
};
TSIMD_DETAIL_CHECK_SIMD_OP(SimdOp<SimdInstruction::SSE3, float>)

TSIMD_NAMESPACE_END
