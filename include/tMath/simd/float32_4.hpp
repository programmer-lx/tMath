#pragma once

#include "check_simd.hpp"

TMATH_SIMD_NAMESPACE_BEGIN

namespace detail
{
    inline __m128 get_abs_mask_impl()
    {
        const __m128 ones = _mm_cmpeq_ps(_mm_setzero_ps(), _mm_setzero_ps()); // 全 1
        const __m128 sign_bit = _mm_set1_ps(-0.0f); // 符号位为 1，其余为 0
        return _mm_andnot_ps(sign_bit, ones); // (!(sign_bit)) & ones
    }

    // 四个分量的符号位都为0，其余位为1
    inline __m128 get_abs_mask()
    {
        static __m128 mask = get_abs_mask_impl();
        return mask;
    }
}


// ============================================ operators ============================================
template<is_simd_float32_4 TSimd>
TSimd& operator+=(TSimd& lhs, TSimd rhs)
{
    lhs.mem = _mm_add_ps(lhs.mem, rhs.mem);
    return lhs;
}

template<is_simd_float32_4 TSimd>
TSimd& operator-=(TSimd& lhs, TSimd rhs)
{
    lhs.mem = _mm_sub_ps(lhs.mem, rhs.mem);
    return lhs;
}



// ============================================ functions ============================================
template<is_simd_float32_4 TSimd>
TSimd abs(TSimd v)
{
    TSimd result;
    result.mem = _mm_and_ps(v.mem, detail::get_abs_mask());
    return result;
}



TMATH_SIMD_NAMESPACE_END
