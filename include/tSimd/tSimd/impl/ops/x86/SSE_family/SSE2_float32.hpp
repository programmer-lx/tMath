#pragma once

#include <emmintrin.h> // SSE2

#include "../../dispatch.hpp"
#include "SSE_float32.hpp"

TSIMD_NAMESPACE_BEGIN

// SSE2 的浮点运算与SSE一致
template<>
struct SimdOp<SimdInstruction::SSE2, float> : SimdOp<SimdInstruction::SSE, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(SSE2, float, __m128, Alignment::SSE_Family)
};
TSIMD_DETAIL_CHECK_SIMD_OP(SimdOp<SimdInstruction::SSE2, float>)

TSIMD_NAMESPACE_END
