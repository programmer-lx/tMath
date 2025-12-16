#pragma once

#include "math_defs_simd.hpp"
#include "platform.hpp"


TMATH_SIMD_NAMESPACE_BEGIN

using float4 = __m128;

inline float4 load(const float* ptr)
{
    return _mm_loadu_ps(ptr);
}

template<TMATH_NAMESPACE_NAME::is_vector4_float TVec4>
inline float4 load(const TVec4& vec)
{
    return _mm_loadu_ps(vec.data);
}

inline void store(float* ptr, float4 v)
{
    return _mm_storeu_ps(ptr, v);
}

template<TMATH_NAMESPACE_NAME::is_vector4_float TVec4>
inline void store(TVec4& vec, float4 v)
{
    _mm_storeu_ps(vec.data, v);
}

inline float4 set(float scalar)
{
    return _mm_set1_ps(scalar);
}

inline float4 set(float x, float y, float z, float w)
{
    return _mm_set_ps(w, z, y, x);
}

inline float4 add(float4 lhs, float4 rhs)
{
    return _mm_add_ps(lhs, rhs);
}

inline float4 sub(float4 lhs, float4 rhs)
{
    return _mm_sub_ps(lhs, rhs);
}

inline float4 mul(float4 lhs, float4 rhs)
{
    return _mm_mul_ps(lhs, rhs);
}

inline float4 mul(float4 lhs, float scalar)
{
    return _mm_mul_ps(lhs, _mm_set1_ps(scalar));
}

inline float4 div(float4 lhs, float4 rhs)
{
    return _mm_div_ps(lhs, rhs);
}

inline float4 div(float4 lhs, float scalar)
{
    return _mm_div_ps(lhs, _mm_set1_ps(scalar));
}

inline float4 mul_add(float4 a, float4 b, float4 c)
{
#if defined(TMATH_HAS_FMA3)
    return _mm_fmadd_ps(a, b, c);
#else
    return _mm_add_ps(_mm_mul_ps(a, b), c);
#endif
}

namespace detail
{
    inline float4 get_abs_mask_impl()
    {
        const float4 ones = _mm_cmpeq_ps(_mm_setzero_ps(), _mm_setzero_ps()); // 全 1
        const float4 sign_bit = _mm_set1_ps(-0.0f); // 符号位为 1，其余为 0
        return _mm_andnot_ps(sign_bit, ones); // (!(sign_bit)) & ones
    }

    // 四个分量的符号位都为0，其余位为1
    inline float4 get_abs_mask()
    {
        static float4 mask = get_abs_mask_impl();
        return mask;
    }
}

inline float4 abs(float4 v)
{
    return _mm_and_ps(v, detail::get_abs_mask());
}

inline float4 sqrt(float4 v)
{
    return _mm_sqrt_ps(v);
}

inline float dot4(float4 lhs, float4 rhs)
{
    float4 mul = _mm_mul_ps(lhs, rhs);                                      // mul      = [w1w2, z1z2, y1y2, x1x2]
    float4 shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 3, 2));     // shuffle  = [y1y2, x1x2, w1w2, z1z2]
    mul = _mm_add_ps(mul, shuffle);                                         // mul      = [w1w2+y1y2, z1z2+x1x2, y1y2+w1w2, x1x2+z1z2]
    shuffle = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(3, 3, 3, 3));            // shuffle  = [N, N, N, w1w2+y1y2]
    mul = _mm_add_ps(shuffle, mul);                                         // mul      = [N, N, N, z1z2+x1x2 + w1w2+y1y2]
    return _mm_cvtss_f32(mul);                                              // pick mul[lane(0)]
}

TMATH_SIMD_NAMESPACE_END
