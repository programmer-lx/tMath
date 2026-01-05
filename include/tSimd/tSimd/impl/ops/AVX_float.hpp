#pragma once

#include "simd_instruction_select.hpp"
#include "func_attr.hpp"

TSIMD_DETAIL_BEGIN_DECL_SIMD_OP(AVX, float, __m256)

TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV load(float const* mem) noexcept
{
    return _mm256_load_ps(mem);
}

TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV loadu(float const* mem) noexcept
{
    return _mm256_loadu_ps(mem);
}

TSIMD_OP_AVX_API static void TSIMD_CALL_CONV store(float* mem, batch_t v) noexcept
{
    _mm256_store_ps(mem, v);
}

TSIMD_OP_AVX_API static void TSIMD_CALL_CONV storeu(float* mem, batch_t v) noexcept
{
    _mm256_storeu_ps(mem, v);
}

TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV zero() noexcept
{
    return _mm256_setzero_ps();
}

TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV set(float x) noexcept
{
    return _mm256_set1_ps(x);
}

TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV add(batch_t lhs, batch_t rhs) noexcept
{
    return _mm256_add_ps(lhs, rhs);
}

TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV sub(batch_t lhs, batch_t rhs) noexcept
{
    return _mm256_sub_ps(lhs, rhs);
}

TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV mul(batch_t lhs, batch_t rhs) noexcept
{
    return _mm256_mul_ps(lhs, rhs);
}

TSIMD_OP_AVX_API static batch_t TSIMD_CALL_CONV div(batch_t lhs, batch_t rhs) noexcept
{
    return _mm256_div_ps(lhs, rhs);
}

TSIMD_OP_AVX_API static float TSIMD_CALL_CONV sum(batch_t v) noexcept
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

TSIMD_DETAIL_END_DECL_SIMD_OP()
