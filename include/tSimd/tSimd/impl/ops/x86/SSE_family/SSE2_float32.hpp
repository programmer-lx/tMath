#pragma once

#include <xmmintrin.h> // SSE
#include <emmintrin.h> // SSE2

#include "../../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::SSE2, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(SSE2, float, __m128)

    TSIMD_OP_SSE2_API static batch_t TSIMD_CALL_CONV load(float const* mem) noexcept
    {
        return _mm_load_ps(mem);
    }

    TSIMD_OP_SSE2_API static batch_t TSIMD_CALL_CONV loadu(float const* mem) noexcept
    {
        return _mm_loadu_ps(mem);
    }

    TSIMD_OP_SSE2_API static void TSIMD_CALL_CONV store(float* mem, batch_t v) noexcept
    {
        _mm_store_ps(mem, v);
    }

    TSIMD_OP_SSE2_API static void TSIMD_CALL_CONV storeu(float* mem, batch_t v) noexcept
    {
        _mm_storeu_ps(mem, v);
    }

    TSIMD_OP_SSE2_API static batch_t TSIMD_CALL_CONV zero() noexcept
    {
        return _mm_setzero_ps();
    }

    TSIMD_OP_SSE2_API static batch_t TSIMD_CALL_CONV set(float x) noexcept
    {
        return _mm_set1_ps(x);
    }

    TSIMD_OP_SSE2_API static batch_t TSIMD_CALL_CONV add(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm_add_ps(lhs, rhs);
    }

    TSIMD_OP_SSE2_API static batch_t TSIMD_CALL_CONV sub(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm_sub_ps(lhs, rhs);
    }

    TSIMD_OP_SSE2_API static batch_t TSIMD_CALL_CONV mul(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm_mul_ps(lhs, rhs);
    }

    TSIMD_OP_SSE2_API static batch_t TSIMD_CALL_CONV div(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm_div_ps(lhs, rhs);
    }

    TSIMD_OP_SSE2_API static float TSIMD_CALL_CONV sum(batch_t v) noexcept
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
};

TSIMD_NAMESPACE_END
