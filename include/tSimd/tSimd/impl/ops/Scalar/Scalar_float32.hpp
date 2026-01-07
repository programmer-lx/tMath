#pragma once

#include "../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::Scalar, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(Scalar, float, float, Alignment::Scalar)

    TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV TSIMD_op_load(const float* mem) noexcept
    {
        return *mem;
    }

    TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV TSIMD_op_loadu(const float* mem) noexcept
    {
        return *mem;
    }

    TSIMD_OP_SCALAR_API static void TSIMD_SCALAR_CALL_CONV TSIMD_op_store(float* mem, batch_t v) noexcept
    {
        *mem = v;
    }

    TSIMD_OP_SCALAR_API static void TSIMD_SCALAR_CALL_CONV TSIMD_op_storeu(float* mem, batch_t v) noexcept
    {
        *mem = v;
    }

    TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV TSIMD_op_zero() noexcept
    {
        return 0.0f;
    }

    TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV TSIMD_op_set(float x) noexcept
    {
        return x;
    }

    TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV TSIMD_op_add(batch_t lhs, batch_t rhs) noexcept
    {
        return lhs + rhs;
    }

    TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV TSIMD_op_sub(batch_t lhs, batch_t rhs) noexcept
    {
        return lhs - rhs;
    }

    TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV TSIMD_op_mul(batch_t lhs, batch_t rhs) noexcept
    {
        return lhs * rhs;
    }

    TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV TSIMD_op_div(batch_t lhs, batch_t rhs) noexcept
    {
        return lhs / rhs;
    }

    TSIMD_OP_SCALAR_API static float TSIMD_SCALAR_CALL_CONV TSIMD_op_reduce_sum(batch_t v) noexcept
    {
        return v;
    }

    TSIMD_OP_SCALAR_API static float TSIMD_SCALAR_CALL_CONV TSIMD_op_mul_add(batch_t a, batch_t b, batch_t c) noexcept
    {
        return a * b + c;
    }
};

TSIMD_NAMESPACE_END
