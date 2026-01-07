#pragma once

#include <xmmintrin.h> // SSE

#include "../../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::SSE, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(SSE, float, __m128, Alignment::SSE_Family)

    TSIMD_OP_SSE_API static batch_t TSIMD_CALL_CONV TSIMD_op_load(const float* mem) noexcept
    {
        return _mm_load_ps(mem);
    }

    TSIMD_OP_SSE_API static batch_t TSIMD_CALL_CONV TSIMD_op_loadu(const float* mem) noexcept
    {
        return _mm_loadu_ps(mem);
    }

    TSIMD_OP_SSE_API static void TSIMD_CALL_CONV TSIMD_op_store(float* mem, batch_t v) noexcept
    {
        _mm_store_ps(mem, v);
    }

    TSIMD_OP_SSE_API static void TSIMD_CALL_CONV TSIMD_op_storeu(float* mem, batch_t v) noexcept
    {
        _mm_storeu_ps(mem, v);
    }

    TSIMD_OP_SSE_API static batch_t TSIMD_CALL_CONV TSIMD_op_zero() noexcept
    {
        return _mm_setzero_ps();
    }

    TSIMD_OP_SSE_API static batch_t TSIMD_CALL_CONV TSIMD_op_set(float x) noexcept
    {
        return _mm_set1_ps(x);
    }

    TSIMD_OP_SSE_API static batch_t TSIMD_CALL_CONV TSIMD_op_add(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm_add_ps(lhs, rhs);
    }

    TSIMD_OP_SSE_API static batch_t TSIMD_CALL_CONV TSIMD_op_sub(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm_sub_ps(lhs, rhs);
    }

    TSIMD_OP_SSE_API static batch_t TSIMD_CALL_CONV TSIMD_op_mul(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm_mul_ps(lhs, rhs);
    }

    TSIMD_OP_SSE_API static batch_t TSIMD_CALL_CONV TSIMD_op_div(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm_div_ps(lhs, rhs);
    }

    TSIMD_OP_SSE_API static float TSIMD_CALL_CONV TSIMD_op_sum(batch_t v) noexcept
    {
        // [d, c, b, a]
        //       +
        // [c, d, a, b]
        //       =
        // [c+d, c+d, a+b, a+b]
        //       +
        // [a+b, a+b, c+d, c+d]
        // [a+b+c+d, ...]
        // get lane[0]

        __m128 t1 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 3, 0, 1));
        t1 = _mm_add_ps(v, t1);
        v = _mm_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 0, 3, 2));
        t1 = _mm_add_ps(t1, v);
        return _mm_cvtss_f32(t1);
    }

    TSIMD_OP_SSE_API static batch_t TSIMD_CALL_CONV TSIMD_op_mul_add(batch_t a, batch_t b, batch_t c) noexcept
    {
        return _mm_add_ps(_mm_mul_ps(a, b), c);
    }
};

TSIMD_NAMESPACE_END
