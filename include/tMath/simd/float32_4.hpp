#pragma once

#include "types.hpp"


TMATH_SIMD_NAMESPACE_BEGIN

// fn: permute
#if defined(TMATH_USE_AVX)
    #define tmath_permute_ps(v, imm8) _mm_permute_ps((v), (imm8))
#elif defined(TMATH_USE_SSE2)
    #define tmath_permute_ps(v, imm8) _mm_shuffle_ps((v), (v), (imm8))
#endif


template<TMATH_NAMESPACE_NAME::is_vector4_float TVec4>
inline float32_4 TMATH_SIMD_CALL_CONV load(const TVec4& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.x, vec.y, vec.z, vec.w };
#else
    return _mm_loadu_ps(vec.data);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector3_float TVec3>
inline float32_4 TMATH_SIMD_CALL_CONV load_point(const TVec3& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.x, vec.y, vec.z, 1.0f };
#else
    return _mm_set_ps(1.0f, vec.z, vec.y, vec.x);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector2_float TVec2>
inline float32_4 TMATH_SIMD_CALL_CONV load_point(const TVec2& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.x, vec.y, 0.0f, 1.0f };
#else
    return _mm_set_ps(1.0f, 0.0f, vec.y, vec.x);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector3_float TVec3>
inline float32_4 TMATH_SIMD_CALL_CONV load_vector(const TVec3& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.x, vec.y, vec.z, 0.0f };
#else
    return _mm_set_ps(0.0f, vec.z, vec.y, vec.x);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector2_float TVec2>
inline float32_4 TMATH_SIMD_CALL_CONV load_vector(const TVec2& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.x, vec.y, 0.0f, 0.0f };
#else
    return _mm_set_ps(0.0f, 0.0f, vec.y, vec.x);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector4_float TVec4>
inline void TMATH_SIMD_CALL_CONV store(TVec4& vec, float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    vec.x = v.x;
    vec.y = v.y;
    vec.z = v.z;
    vec.w = v.w;
#else
    _mm_storeu_ps(vec.data, v);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector3_float TVec3>
inline void TMATH_SIMD_CALL_CONV store(TVec3& vec, float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    vec.x = v.x;
    vec.y = v.y;
    vec.z = v.z;
#else
    alignas(16) float tmp[4];
    _mm_store_ps(tmp, v);
    vec.x = tmp[0];
    vec.y = tmp[1];
    vec.z = tmp[2];
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector2_float TVec2>
inline void TMATH_SIMD_CALL_CONV store(TVec2& vec, float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    vec.x = v.x;
    vec.y = v.y;
#else
    alignas(16) float tmp[4];
    _mm_store_ps(tmp, v);
    vec.x = tmp[0];
    vec.y = tmp[1];
#endif
}

inline float TMATH_SIMD_CALL_CONV get_x(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.x;
#else
    return _mm_cvtss_f32(v);
#endif
}

inline float TMATH_SIMD_CALL_CONV get_y(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.y;
#else
    return _mm_cvtss_f32(tmath_permute_ps(v, _MM_SHUFFLE(1, 1, 1, 1)));
#endif
}

inline float TMATH_SIMD_CALL_CONV get_z(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.z;
#else
    return _mm_cvtss_f32(tmath_permute_ps(v, _MM_SHUFFLE(2, 2, 2, 2)));
#endif
}

inline float TMATH_SIMD_CALL_CONV get_w(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.w;
#else
    return _mm_cvtss_f32(tmath_permute_ps(v, _MM_SHUFFLE(3, 3, 3, 3)));
#endif
}


inline float32_4 TMATH_SIMD_CALL_CONV set(float scalar) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { scalar, scalar, scalar, scalar };
#else
    return _mm_set1_ps(scalar);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV set(float x, float y, float z, float w) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { x, y, z, w };
#else
    return _mm_set_ps(w, z, y, x);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV add(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    return lhs + rhs;
#else
    return _mm_add_ps(lhs, rhs);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV sub(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    return lhs - rhs;
#else
    return _mm_sub_ps(lhs, rhs);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV mul(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    return {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z,
        lhs.w * rhs.w
    };
#else
    return _mm_mul_ps(lhs, rhs);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV mul(float32_4_arg_in lhs, float scalar) noexcept
{
#if defined(TMATH_NO_SIMD)
    return lhs * scalar;
#else
    return _mm_mul_ps(lhs, _mm_set1_ps(scalar));
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV div(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    return {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z,
        lhs.w / rhs.w
    };
#else
    return _mm_div_ps(lhs, rhs);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV div(float32_4_arg_in lhs, float scalar) noexcept
{
#if defined(TMATH_NO_SIMD)
    return lhs / scalar;
#else
    return _mm_div_ps(lhs, _mm_set1_ps(scalar));
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV mul_add(float32_4_arg_in a, float32_4_arg_in b, float32_4_arg_in c) noexcept
{
#if defined(TMATH_NO_SIMD)
    float32_4 mul = {
        a.x * b.x,
        a.y * b.y,
        a.z * b.z,
        a.w * b.w
    };
    return mul + c;
#elif defined(TMATH_USE_FMA3)
    return _mm_fmadd_ps(a, b, c);
#else
    return _mm_add_ps(_mm_mul_ps(a, b), c);
#endif
}

namespace detail
{
#if !defined(TMATH_NO_SIMD)
    inline float32_4 TMATH_SIMD_CALL_CONV get_abs_mask_impl() noexcept
    {
        const float32_4 ones = _mm_cmpeq_ps(_mm_setzero_ps(), _mm_setzero_ps()); // 全 1
        const float32_4 sign_bit = _mm_set1_ps(-0.0f); // 符号位为 1，其余为 0
        return _mm_andnot_ps(sign_bit, ones); // (!(sign_bit)) & ones
    }

    // 四个分量的符号位都为0，其余位为1
    inline float32_4 TMATH_SIMD_CALL_CONV get_abs_mask() noexcept
    {
        static float32_4 mask = get_abs_mask_impl();
        return mask;
    }
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV abs(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::abs(v);
#else
    return _mm_and_ps(v, detail::get_abs_mask());
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV sqrt(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return {
        TMATH_NAMESPACE_NAME::sqrt(v.x),
        TMATH_NAMESPACE_NAME::sqrt(v.y),
        TMATH_NAMESPACE_NAME::sqrt(v.z),
        TMATH_NAMESPACE_NAME::sqrt(v.w)
    };
#else
    return _mm_sqrt_ps(v);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV sin(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::sin(v);
#else
    return _mm_sin_ps(v);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV cos(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::cos(v);
#else
    return _mm_cos_ps(v);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV dot2(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    float value = TMATH_NAMESPACE_NAME::dot(lhs.x, lhs.y, rhs.x, rhs.y);
    return { value, value, value, value };
#elif defined(TMATH_USE_SSE4_1)
    return _mm_dp_ps(lhs, rhs, 0x3f);                   // imm8: 7-4: compute, 3-0: store
#elif defined(TMATH_USE_SSE3)
    float32_4 temp = _mm_mul_ps(lhs, rhs);              // temp = [             w1w2,             z1z2,             y1y2,             x1x2]
    temp = _mm_and_ps(temp, Mask128::Lane01.f32_4);     // temp = [                0,             z1z2,             y1y2,             x1x2]
    temp = _mm_hadd_ps(temp, temp);                     // temp = [             z1z2,        x1x2+y1y2,             z1z2,        x1x2+y1y2]
    return _mm_hadd_ps(temp, temp);                     // temp = [ x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2]
#elif defined(TMATH_USE_SSE2)
    float32_4 mul = _mm_mul_ps(lhs, rhs);                                   // mul      = [     w1w2,      z1z2, y1y2,                  x1x2]
    mul = _mm_and_ps(mul, Mask128::Lane01.f32_4);                           // mul      = [        0,         0, y1y2,                  x1x2]
    float32_4 shuffle = tmath_permute_ps(mul, _MM_SHUFFLE(1, 0, 3, 2));     // shuffle  = [     y1y2,      x1x2,    0,                     0]
    mul = _mm_add_ps(mul, shuffle);                                         // mul      = [     y1y2,      x1x2, y1y2,                  x1x2]
    shuffle = tmath_permute_ps(mul, _MM_SHUFFLE(3, 3, 3, 3));               // shuffle  = [        N,         N,    N,                  y1y2]
    mul = _mm_add_ps(shuffle, mul);                                         // mul      = [        N,         N,    N,           x1x2 + y1y2]
    return tmath_permute_ps(mul, _MM_SHUFFLE(0, 0, 0, 0));                  // splat mul[lane(0)]
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV dot3(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    float value = TMATH_NAMESPACE_NAME::dot(lhs.x, lhs.y, lhs.z, rhs.x, rhs.y, rhs.z);
    return { value, value, value, value };
#elif defined(TMATH_USE_SSE4_1)
    return _mm_dp_ps(lhs, rhs, 0x7f);                   // imm8: 7-4: compute, 3-0: store
#elif defined(TMATH_USE_SSE3)
    float32_4 temp = _mm_mul_ps(lhs, rhs);              // temp = [             w1w2,             z1z2,             y1y2,             x1x2]
    temp = _mm_and_ps(temp, Mask128::Lane012.f32_4);    // temp = [                0,             z1z2,             y1y2,             x1x2]
    temp = _mm_hadd_ps(temp, temp);                     // temp = [             z1z2,        x1x2+y1y2,             z1z2,        x1x2+y1y2]
    return _mm_hadd_ps(temp, temp);                     // temp = [ x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2]
#elif defined(TMATH_USE_SSE2)
    float32_4 mul = _mm_mul_ps(lhs, rhs);                                   // mul      = [     w1w2,      z1z2, y1y2,                  x1x2]
    mul = _mm_and_ps(mul, Mask128::Lane012.f32_4);                          // mul      = [        0,      z1z2, y1y2,                  x1x2]
    float32_4 shuffle = tmath_permute_ps(mul, _MM_SHUFFLE(1, 0, 3, 2));     // shuffle  = [     y1y2,      x1x2,    0,                  z1z2]
    mul = _mm_add_ps(mul, shuffle);                                         // mul      = [     y1y2, z1z2+x1x2, y1y2,             x1x2+z1z2]
    shuffle = tmath_permute_ps(mul, _MM_SHUFFLE(3, 3, 3, 3));               // shuffle  = [        N,         N,    N,                  y1y2]
    mul = _mm_add_ps(shuffle, mul);                                         // mul      = [        N,         N,    N,      z1z2+x1x2 + y1y2]
    return tmath_permute_ps(mul, _MM_SHUFFLE(0, 0, 0, 0));                  // splat mul[lane(0)]
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV dot4(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    float value = TMATH_NAMESPACE_NAME::dot(lhs, rhs);
    return { value, value, value, value };
#elif defined(TMATH_USE_SSE4_1)
    return _mm_dp_ps(lhs, rhs, 0xff);
#elif defined(TMATH_USE_SSE3)
    float32_4 temp = _mm_mul_ps(lhs, rhs);  // temp = [                 w1w2,                  z1z2,                  y1y2,                  x1x2]
    temp = _mm_hadd_ps(temp, temp);         // temp = [            z1z2+w1w2,             x1x2+y1y2,             z1z2+w1w2,             x1x2+y1y2]
    return _mm_hadd_ps(temp, temp);         // temp = [x1x2+y1y2 + z1z2+w1w2, x1x2+y1y2 + z1z2+w1w2, x1x2+y1y2 + z1z2+w1w2, x1x2+y1y2 + z1z2+w1w2]
#elif defined(TMATH_USE_SSE2)
    float32_4 mul = _mm_mul_ps(lhs, rhs);                                   // mul      = [      w1w2,       z1z2,      y1y2,                  x1x2]
    float32_4 shuffle = tmath_permute_ps(mul, _MM_SHUFFLE(1, 0, 3, 2));     // shuffle  = [      y1y2,       x1x2,      w1w2,                  z1z2]
    mul = _mm_add_ps(mul, shuffle);                                         // mul      = [ w1w2+y1y2,  z1z2+x1x2, y1y2+w1w2,             x1x2+z1z2]
    shuffle = tmath_permute_ps(mul, _MM_SHUFFLE(3, 3, 3, 3));               // shuffle  = [         N,          N,         N,             w1w2+y1y2]
    mul = _mm_add_ps(shuffle, mul);                                         // mul      = [         N,          N,         N, z1z2+x1x2 + w1w2+y1y2]
    return tmath_permute_ps(mul, _MM_SHUFFLE(0, 0, 0, 0));                  // splat mul[lane(0)]
#endif
}

/**
 * @return (x1y2 - x2y1, same...)
 */
inline float32_4 TMATH_SIMD_CALL_CONV cross2(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    float value = TMATH_NAMESPACE_NAME::cross(lhs.x, lhs.y, rhs.x, rhs.y);
    return { value, value, value, value };
#elif defined(TMATH_USE_SSE2)
    float32_4 t2 = tmath_permute_ps(rhs, _MM_SHUFFLE(0, 1, 0, 1));      // t2 = [N, N,   x2,          y2]
    float32_4 t1 = _mm_mul_ps(lhs, t2);                                 // t1 = [N, N, x2y1,        x1y2]
    t2 = tmath_permute_ps(t1, _MM_SHUFFLE(1, 1, 1, 1));                 // t2 = [N, N, x2y1,        x2y1]
    t1 = _mm_sub_ps(t1, t2);                                            // t1 = [N, N,    N,   x1y2-x2y1]
    return tmath_permute_ps(t1, _MM_SHUFFLE(0, 0, 0, 0));               // re = [x1y2-x2y1, x1y2-x2y1, x1y2-x2y1, x1y2-x2y1]
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV cross3(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    float value = TMATH_NAMESPACE_NAME::cross(lhs.x, lhs.y, rhs.x, rhs.y);
    return { value, value, value, value };
#elif defined(TMATH_USE_SSE2)
    return _mm_set1_ps(0.0f); // TODO cross3
#endif
}

TMATH_SIMD_NAMESPACE_END
