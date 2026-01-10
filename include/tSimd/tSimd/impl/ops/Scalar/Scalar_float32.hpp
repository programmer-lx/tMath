#pragma once

#include "_Scalar_types.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::Scalar, float32>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(Scalar, float32, Scalar::Batch<float32>, Alignment::Scalar)

    TSIMD_OP_SIG_SCALAR(batch_t, load, (const float32* mem))
    {
        return { *mem };
    }

    TSIMD_OP_SIG_SCALAR(batch_t, loadu, (const float32* mem))
    {
        return { *mem };
    }

    TSIMD_OP_SIG_SCALAR(void, store, (float32* mem, batch_t v))
    {
        *mem = v.v;
    }

    TSIMD_OP_SIG_SCALAR(void, storeu, (float32* mem, batch_t v))
    {
        *mem = v.v;
    }

    TSIMD_OP_SIG_SCALAR(batch_t, zero, ())
    {
        return { 0.0f };
    }

    TSIMD_OP_SIG_SCALAR(batch_t, set, (float32 x))
    {
        return { x };
    }

    TSIMD_OP_SIG_SCALAR(batch_t, add, (batch_t lhs, batch_t rhs))
    {
        return { lhs.v + rhs.v };
    }

    TSIMD_OP_SIG_SCALAR(batch_t, sub, (batch_t lhs, batch_t rhs))
    {
        return { lhs.v - rhs.v };
    }

    TSIMD_OP_SIG_SCALAR(batch_t, mul, (batch_t lhs, batch_t rhs))
    {
        return { lhs.v * rhs.v };
    }

    TSIMD_OP_SIG_SCALAR(batch_t, div, (batch_t lhs, batch_t rhs))
    {
        return { lhs.v / rhs.v };
    }

    TSIMD_OP_SIG_SCALAR(float32, reduce_sum, (batch_t v))
    {
        return v.v;
    }

    TSIMD_OP_SIG_SCALAR(batch_t, mul_add, (batch_t a, batch_t b, batch_t c))
    {
        return { a.v * b.v + c.v };
    }
};

TSIMD_DETAIL_CHECK_SCALAR_OP(SimdOp<SimdInstruction::Scalar, float32>);

TSIMD_NAMESPACE_END
