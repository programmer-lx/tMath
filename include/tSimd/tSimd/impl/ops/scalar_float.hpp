#pragma once

#include "simd_instruction_select.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::Scalar, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(Scalar, float, float)

    static TSIMD_FORCE_INLINE batch_t TSIMD_SCALAR_CALL_CONV load(float const* mem) noexcept
    {
        return *mem;
    }

    static TSIMD_FORCE_INLINE batch_t TSIMD_SCALAR_CALL_CONV loadu(float const* mem) noexcept
    {
        return *mem;
    }

    static TSIMD_FORCE_INLINE void TSIMD_SCALAR_CALL_CONV store(float* mem, batch_t v) noexcept
    {
        *mem = v;
    }

    static TSIMD_FORCE_INLINE void TSIMD_CALL_CONV storeu(float* mem, batch_t v) noexcept
    {
        *mem = v;
    }

    static TSIMD_FORCE_INLINE batch_t TSIMD_SCALAR_CALL_CONV zero() noexcept
    {
        return 0.0f;
    }

    static TSIMD_FORCE_INLINE batch_t TSIMD_SCALAR_CALL_CONV set(float x) noexcept
    {
        return x;
    }

    static TSIMD_FORCE_INLINE batch_t TSIMD_SCALAR_CALL_CONV add(batch_t lhs, batch_t rhs) noexcept
    {
        return lhs + rhs;
    }

    static TSIMD_FORCE_INLINE batch_t TSIMD_SCALAR_CALL_CONV sub(batch_t lhs, batch_t rhs) noexcept
    {
        return lhs - rhs;
    }

    static TSIMD_FORCE_INLINE batch_t TSIMD_SCALAR_CALL_CONV mul(batch_t lhs, batch_t rhs) noexcept
    {
        return lhs * rhs;
    }

    static TSIMD_FORCE_INLINE batch_t TSIMD_SCALAR_CALL_CONV div(batch_t lhs, batch_t rhs) noexcept
    {
        return lhs / rhs;
    }

    static TSIMD_FORCE_INLINE float TSIMD_SCALAR_CALL_CONV sum(batch_t v) noexcept
    {
        return v;
    }
};

TSIMD_NAMESPACE_END
