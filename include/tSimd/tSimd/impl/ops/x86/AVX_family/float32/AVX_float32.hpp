#pragma once

#include "../_AVX_family_types.hpp"

TSIMD_NAMESPACE_BEGIN

template<>
struct SimdOp<SimdInstruction::AVX, float32>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(AVX, float32, AVX_family::Batch<float32>, Alignment::AVX_Family)

    TSIMD_OP_SIG_AVX(batch_t, load, (const float32* mem))
    {
        return { _mm256_load_ps(mem) };
    }

    TSIMD_OP_SIG_AVX(batch_t, loadu, (const float32* mem))
    {
        return { _mm256_loadu_ps(mem) };
    }

    TSIMD_OP_SIG_AVX(void, store, (float32* mem, batch_t v))
    {
        _mm256_store_ps(mem, v.v);
    }

    TSIMD_OP_SIG_AVX(void, storeu, (float32* mem, batch_t v))
    {
        _mm256_storeu_ps(mem, v.v);
    }

    TSIMD_OP_SIG_AVX(batch_t, zero, ())
    {
        return { _mm256_setzero_ps() };
    }

    TSIMD_OP_SIG_AVX(batch_t, set, (float32 x))
    {
        return { _mm256_set1_ps(x) };
    }

    TSIMD_OP_SIG_AVX(batch_t, add, (batch_t lhs, batch_t rhs))
    {
        return { _mm256_add_ps(lhs.v, rhs.v) };
    }

    TSIMD_OP_SIG_AVX(batch_t, sub, (batch_t lhs, batch_t rhs))
    {
        return { _mm256_sub_ps(lhs.v, rhs.v) };
    }

    TSIMD_OP_SIG_AVX(batch_t, mul, (batch_t lhs, batch_t rhs))
    {
        return { _mm256_mul_ps(lhs.v, rhs.v) };
    }

    TSIMD_OP_SIG_AVX(batch_t, div, (batch_t lhs, batch_t rhs))
    {
        return { _mm256_div_ps(lhs.v, rhs.v) };
    }

    TSIMD_OP_SIG_AVX(float32, reduce_sum, (batch_t v))
    {
        // [8, 7, 6, 5, 4, 3, 2, 1]
        // hadd
        // [78, 56, 78, 56, 34, 12, 34, 12]
        // hadd
        // [5678, 5678, 5678, 5678, 1234, 1234, 1234, 1234]
        __m256 t1 = _mm256_hadd_ps(v.v, v.v);
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

    TSIMD_OP_SIG_AVX(batch_t, mul_add, (batch_t a, batch_t b, batch_t c))
    {
        return { _mm256_add_ps(_mm256_mul_ps(a.v, b.v), c.v) };
    }
};
TSIMD_DETAIL_CHECK_SIMD_OP(SimdOp<SimdInstruction::AVX, float32>);

TSIMD_NAMESPACE_END
