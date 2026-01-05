#pragma once

#include "simd_instruction_select.hpp"
#include "func_attr.hpp"

TSIMD_DETAIL_BEGIN_DECL_SIMD_OP(Scalar, float, float)

TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV load(float const* mem) noexcept
{
    return *mem;
}

TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV loadu(float const* mem) noexcept
{
    return *mem;
}

TSIMD_OP_SCALAR_API static void TSIMD_SCALAR_CALL_CONV store(float* mem, batch_t v) noexcept
{
    *mem = v;
}

TSIMD_OP_SCALAR_API static void TSIMD_CALL_CONV storeu(float* mem, batch_t v) noexcept
{
    *mem = v;
}

TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV zero() noexcept
{
    return 0.0f;
}

TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV set(float x) noexcept
{
    return x;
}

TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV add(batch_t lhs, batch_t rhs) noexcept
{
    return lhs + rhs;
}

TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV sub(batch_t lhs, batch_t rhs) noexcept
{
    return lhs - rhs;
}

TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV mul(batch_t lhs, batch_t rhs) noexcept
{
    return lhs * rhs;
}

TSIMD_OP_SCALAR_API static batch_t TSIMD_SCALAR_CALL_CONV div(batch_t lhs, batch_t rhs) noexcept
{
    return lhs / rhs;
}

TSIMD_OP_SCALAR_API static float TSIMD_SCALAR_CALL_CONV sum(batch_t v) noexcept
{
    return v;
}

TSIMD_DETAIL_END_DECL_SIMD_OP()
