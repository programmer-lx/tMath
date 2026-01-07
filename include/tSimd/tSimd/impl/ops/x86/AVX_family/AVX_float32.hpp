#pragma once

#include <immintrin.h> // AVX

#include "../../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::AVX, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(AVX, float, __m256, Alignment::AVX_Family)

    TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV TSIMD_op_load(const float* mem) noexcept
    {
        return _mm256_load_ps(mem);
    }

    TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV TSIMD_op_loadu(const float* mem) noexcept
    {
        return _mm256_loadu_ps(mem);
    }

    TSIMD_OP_AVX_API static void TSIMD_CALL_CONV TSIMD_op_store(float* mem, batch_t v) noexcept
    {
        _mm256_store_ps(mem, v);
    }

    TSIMD_OP_AVX_API static void TSIMD_CALL_CONV TSIMD_op_storeu(float* mem, batch_t v) noexcept
    {
        _mm256_storeu_ps(mem, v);
    }

    TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV TSIMD_op_zero() noexcept
    {
        return _mm256_setzero_ps();
    }

    TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV TSIMD_op_set(float x) noexcept
    {
        return _mm256_set1_ps(x);
    }

    TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV TSIMD_op_add(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm256_add_ps(lhs, rhs);
    }

    TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV TSIMD_op_sub(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm256_sub_ps(lhs, rhs);
    }

    TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV TSIMD_op_mul(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm256_mul_ps(lhs, rhs);
    }

    TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV TSIMD_op_div(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm256_div_ps(lhs, rhs);
    }

    TSIMD_OP_AVX_API static float TSIMD_CALL_CONV TSIMD_op_reduce_sum(batch_t v) noexcept
    {
        // [8, 7, 6, 5, 4, 3, 2, 1]
        // hadd
        // [78, 56, 78, 56, 34, 12, 34, 12]
        // hadd
        // [5678, 5678, 5678, 5678, 1234, 1234, 1234, 1234]
        __m256 t1 = _mm256_hadd_ps(v, v);
        t1 = _mm256_hadd_ps(t1, t1);

        // low = [1234, 1234, 1234, 1234]
        // high = [5678, 5678, 5678, 5678]
        __m128 low = _mm256_castps256_ps128(t1);
        __m128 high = _mm256_extractf128_ps(t1, 0b1);

        // add
        // [12345678, ....]
        // get lane[0]
        low = _mm_add_ps(low, high);
        return _mm_cvtss_f32(low);
    }

    TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV TSIMD_op_mul_add(batch_t a, batch_t b, batch_t c) noexcept
    {
        return _mm256_add_ps(_mm256_mul_ps(a, b), c);
    }
};

TSIMD_NAMESPACE_END
