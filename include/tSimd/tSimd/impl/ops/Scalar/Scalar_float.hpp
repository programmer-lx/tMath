#pragma once

#include "../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::Scalar, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(Scalar, float, float, Alignment::Scalar)

    TSIMD_OP_SIG_SCALAR(batch_t, load, (const float* mem))
    {
        return *mem;
    }

    TSIMD_OP_SIG_SCALAR(batch_t, loadu, (const float* mem))
    {
        return *mem;
    }

    TSIMD_OP_SIG_SCALAR(void, store, (float* mem, batch_t v))
    {
        *mem = v;
    }

    TSIMD_OP_SIG_SCALAR(void, storeu, (float* mem, batch_t v))
    {
        *mem = v;
    }

    TSIMD_OP_SIG_SCALAR(float, zero, ())
    {
        return 0.0f;
    }

    TSIMD_OP_SIG_SCALAR(batch_t, set, (float x))
    {
        return x;
    }

    TSIMD_OP_SIG_SCALAR(batch_t, add, (batch_t lhs, batch_t rhs))
    {
        return lhs + rhs;
    }

    TSIMD_OP_SIG_SCALAR(batch_t, sub, (batch_t lhs, batch_t rhs))
    {
        return lhs - rhs;
    }

    TSIMD_OP_SIG_SCALAR(batch_t, mul, (batch_t lhs, batch_t rhs))
    {
        return lhs * rhs;
    }

    TSIMD_OP_SIG_SCALAR(batch_t, div, (batch_t lhs, batch_t rhs))
    {
        return lhs / rhs;
    }

    TSIMD_OP_SIG_SCALAR(float, reduce_sum, (batch_t v))
    {
        return v;
    }

    TSIMD_OP_SIG_SCALAR(batch_t, mul_add, (batch_t a, batch_t b, batch_t c))
    {
        return a * b + c;
    }
};

TSIMD_DETAIL_CHECK_SCALAR_OP(SimdOp<SimdInstruction::Scalar, float>);

TSIMD_NAMESPACE_END
