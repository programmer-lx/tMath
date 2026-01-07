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

    TSIMD_OP_AVX2_FMA3_API static batch_t TSIMD_CALL_CONV TSIMD_op_mul_add(batch_t a, batch_t b, batch_t c) noexcept
    {
        return _mm256_fmadd_ps(a, b, c);
    }
};

TSIMD_NAMESPACE_END
