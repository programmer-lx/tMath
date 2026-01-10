#pragma once

#include "SSE_float32.hpp"

TSIMD_NAMESPACE_BEGIN

// SSE2 的浮点运算与SSE一致
template<>
struct SimdOp<SimdInstruction::SSE2, float32> : SimdOp<SimdInstruction::SSE, float32>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(SSE2, float32, SSE_family::Batch<float32>, Alignment::SSE_Family)
};
TSIMD_DETAIL_CHECK_SIMD_OP(SimdOp<SimdInstruction::SSE2, float32>);

TSIMD_NAMESPACE_END
