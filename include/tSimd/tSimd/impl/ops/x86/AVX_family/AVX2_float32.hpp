#pragma once

#include <immintrin.h> // AVX

#include "../../dispatch.hpp"
#include "AVX_float32.hpp"

TSIMD_NAMESPACE_BEGIN

// AVX2与AVX的浮点运算指令一致
template<>
struct SimdOp<SimdInstruction::AVX2, float> : SimdOp<SimdInstruction::AVX, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(AVX2, float, __m256, Alignment::AVX_Family)
};

TSIMD_NAMESPACE_END
