#pragma once

#include "SSE3_float32.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::SSE4_1, float32> : SimdOp<SimdInstruction::SSE3, float32>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(SSE4_1, float32, SSE_family::Batch<float32>, Alignment::SSE_Family)
};
TSIMD_DETAIL_CHECK_SIMD_OP(SimdOp<SimdInstruction::SSE4_1, float32>);

TSIMD_NAMESPACE_END
