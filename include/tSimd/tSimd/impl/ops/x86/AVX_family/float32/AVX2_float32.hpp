#pragma once

#include "AVX_float32.hpp"

TSIMD_NAMESPACE_BEGIN

// AVX2与AVX的浮点运算指令一致
template<>
struct SimdOp<SimdInstruction::AVX2, float32> : SimdOp<SimdInstruction::AVX, float32>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(AVX2, float32, AVX_family::Batch<float32>, Alignment::AVX_Family)
};
TSIMD_DETAIL_CHECK_SIMD_OP(SimdOp<SimdInstruction::AVX, float32>);

TSIMD_NAMESPACE_END
