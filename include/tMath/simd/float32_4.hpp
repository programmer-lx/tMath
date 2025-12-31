#pragma once

#include "types.hpp"

TMATH_SIMD_NAMESPACE_BEGIN

// fn: permute
#if defined(TMATH_USE_AVX)
    #define tmath_permute_ps(v, imm8) _mm_permute_ps((v), (imm8))
#elif defined(TMATH_USE_SSE2)
    #define tmath_permute_ps(v, imm8) _mm_shuffle_ps((v), (v), (imm8))
#endif

// fn: fmadd
#if defined(TMATH_USE_FMA3)
    #define tmath_fmadd_ps(a, b, c) _mm_fmadd_ps((a), (b), (c))
#elif defined(TMATH_USE_SSE2)
    #define tmath_fmadd_ps(a, b, c) _mm_add_ps(_mm_mul_ps((a), (b)), (c))
#endif


template<TMATH_NAMESPACE_NAME::is_vector4_float TVec4f>
inline float32_4 TMATH_SIMD_CALL_CONV load(const TVec4f& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.data[0], vec.data[1], vec.data[2], vec.data[3] };
#else
    if constexpr (alignof(TVec4f) == _128_alignment)
    {
        return _mm_load_ps(&vec.data[0]);
    }
    else
    {
        return _mm_loadu_ps(&vec.data[0]);
    }
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV load(float scalar) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { scalar, scalar, scalar, scalar };
#else
    return _mm_set1_ps(scalar);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV load(float x, float y, float z, float w) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { x, y, z, w };
#else
    return _mm_set_ps(w, z, y, x);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector3_float TVec3f>
float32_4 TMATH_SIMD_CALL_CONV load_point(const TVec3f& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.data[0], vec.data[1], vec.data[2], 1.0f };
#else
    return _mm_set_ps(1.0f, vec.data[2], vec.data[1], vec.data[0]);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector2_float TVec2f>
float32_4 TMATH_SIMD_CALL_CONV load_point(const TVec2f& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.data[0], vec.data[1], 0.0f, 1.0f };
#else
    return _mm_set_ps(1.0f, 0.0f, vec.data[1], vec.data[0]);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector3_float TVec3f>
float32_4 TMATH_SIMD_CALL_CONV load_vector(const TVec3f& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.data[0], vec.data[1], vec.data[2], 0.0f };
#else
    return _mm_set_ps(0.0f, vec.data[2], vec.data[1], vec.data[0]);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector2_float TVec2f>
float32_4 TMATH_SIMD_CALL_CONV load_vector(const TVec2f& vec) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { vec.data[0], vec.data[1], 0.0f, 0.0f };
#else
    return _mm_set_ps(0.0f, 0.0f, vec.data[1], vec.data[0]);
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector4_float TVec4f>
void TMATH_SIMD_CALL_CONV store(TVec4f& vec, float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    vec.data[0] = v.data[0];
    vec.data[1] = v.data[1];
    vec.data[2] = v.data[2];
    vec.data[3] = v.data[3];
#else
    if constexpr (alignof(TVec4f) == _128_alignment)
    {
        _mm_store_ps(&vec.data[0], v);
    }
    else
    {
        _mm_storeu_ps(&vec.data[0], v);
    }
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector3_float TVec3f>
void TMATH_SIMD_CALL_CONV store(TVec3f& vec, float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    vec.data[0] = v.data[0];
    vec.data[1] = v.data[1];
    vec.data[2] = v.data[2];
#else
    alignas(16) float tmp[4];
    _mm_store_ps(tmp, v);
    vec.data[0] = tmp[0];
    vec.data[1] = tmp[1];
    vec.data[2] = tmp[2];
#endif
}

template<TMATH_NAMESPACE_NAME::is_vector2_float TVec2f>
void TMATH_SIMD_CALL_CONV store(TVec2f& vec, float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    vec.data[0] = v.data[0];
    vec.data[1] = v.data[1];
#else
    alignas(16) float tmp[4];
    _mm_store_ps(tmp, v);
    vec.data[0] = tmp[0];
    vec.data[1] = tmp[1];
#endif
}

inline float TMATH_SIMD_CALL_CONV get_x(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.data[0];
#else
    return _mm_cvtss_f32(v);
#endif
}

inline float TMATH_SIMD_CALL_CONV get_y(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.data[1];
#else
    return _mm_cvtss_f32(tmath_permute_ps(v, _MM_SHUFFLE(1, 1, 1, 1)));
#endif
}

inline float TMATH_SIMD_CALL_CONV get_z(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.data[2];
#else
    return _mm_cvtss_f32(tmath_permute_ps(v, _MM_SHUFFLE(2, 2, 2, 2)));
#endif
}

inline float TMATH_SIMD_CALL_CONV get_w(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.data[3];
#else
    return _mm_cvtss_f32(tmath_permute_ps(v, _MM_SHUFFLE(3, 3, 3, 3)));
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

/**
 * 测试a和b的各个分量是否近似相等
 * @return 如果该分量近似相等，则该分量全部bit置为1，否则为0
 */
inline float32_4 TMATH_SIMD_CALL_CONV approximately_cwise(float32_4_arg_in a, float32_4_arg_in b, float tolerance) noexcept
{
    // 对于单个分量: c = ( abs(a - b) <= tolerance ) ? 0xffffffff : 0

#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::approximately_cwise(a, b, tolerance);
#else
    float32_4 tolerance_4 = _mm_set1_ps(tolerance);
    float32_4 diff = _mm_sub_ps(a, b);
    float32_4 abs_diff = _mm_and_ps(diff, Mask128::Abs4.f32_4);
    return _mm_cmple_ps(abs_diff, tolerance_4);
#endif
}

/**
 * 如果全部分量都近似相等，就返回true，否则返回false
 */
inline bool TMATH_SIMD_CALL_CONV approximately_all(float32_4_arg_in a, float32_4_arg_in b, float tolerance) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::approximately_all(a, b, tolerance);
#else
    float32_4 tolerance_4 = _mm_set1_ps(tolerance);
    float32_4 diff = _mm_sub_ps(a, b);
    float32_4 abs_diff = _mm_and_ps(diff, Mask128::Abs4.f32_4);
    float32_4 mask = _mm_cmple_ps(abs_diff, tolerance_4);

    // movemask: 提取四个lane的最高位，拼接成一个新的整数，返回的整数只有0-3bit是有效的
    return _mm_movemask_ps(mask) == 0xf;
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV min(float32_4_arg_in a, float32_4_arg_in b) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::min(a, b);
#else
    return _mm_min_ps(a, b);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV max(float32_4_arg_in a, float32_4_arg_in b) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::max(a, b);
#else
    return _mm_max_ps(a, b);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV clamp(float32_4_arg_in v, float32_4_arg_in min, float32_4_arg_in max) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::clamp(v, min, max);
#else
    return _mm_min_ps(_mm_max_ps(v, min), max);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV lerp(float32_4_arg_in a, float32_4_arg_in b, float t) noexcept
{
    // result = a + (b - a) * t
    // fmadd: t * (b - a) + a

#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::lerp(a, b, t);
#else
    float32_4 b_a = _mm_sub_ps(b, a);
    float32_4 t2 = _mm_set1_ps(t);
    return tmath_fmadd_ps(t2, b_a, a);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV cwise_mul(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::cwise_mul(lhs, rhs);
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

inline float32_4 TMATH_SIMD_CALL_CONV cwise_div(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    return tMath::cwise_div(lhs, rhs);
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
    return tMath::cwise_mul(a, b) + c;
#else
    return tmath_fmadd_ps(a, b, c);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV abs(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::abs(v);
#else
    return _mm_and_ps(v, Mask128::Abs4.f32_4);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV sqrt(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return {
        std::sqrt(v.data[0]),
        std::sqrt(v.data[1]),
        std::sqrt(v.data[2]),
        std::sqrt(v.data[3])
    };
#elif defined(TMATH_USE_SVML)
    return _mm_svml_sqrt_ps(v);
#else
    return _mm_sqrt_ps(v);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV sin(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::sin(v);
#elif defined(TMATH_USE_SVML)
    return _mm_sin_ps(v);
#else
    // TODO sin
    return _mm_set1_ps(0.0f);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV cos(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::cos(v);
#elif defined(TMATH_USE_SVML)
    return _mm_cos_ps(v);
#else
    // TODO cos
    return _mm_set1_ps(0.0f);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV dot2(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    float value = TMATH_NAMESPACE_NAME::dot(lhs.data[0], lhs.data[1], rhs.data[0], rhs.data[1]);
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
    float value = TMATH_NAMESPACE_NAME::dot(lhs.data[0], lhs.data[1], lhs.data[2], rhs.data[0], rhs.data[1], rhs.data[2]);
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
    float32_4 t1 = _mm_mul_ps(lhs, rhs);  // t1 = [                 w1w2,                  z1z2,                  y1y2,                  x1x2]
    float32_4 t2 = _mm_hadd_ps(t1, t1);   // t2 = [            z1z2+w1w2,             x1x2+y1y2,             z1z2+w1w2,             x1x2+y1y2]
    return _mm_hadd_ps(t2, t2);           // re = [x1x2+y1y2 + z1z2+w1w2, x1x2+y1y2 + z1z2+w1w2, x1x2+y1y2 + z1z2+w1w2, x1x2+y1y2 + z1z2+w1w2]
#elif defined(TMATH_USE_SSE2)
    // float32_4 mul = _mm_mul_ps(lhs, rhs);                                   // mul      = [      w1w2,       z1z2,      y1y2,                  x1x2]
    // float32_4 shuffle = tmath_permute_ps(mul, _MM_SHUFFLE(1, 0, 3, 2));     // shuffle  = [      y1y2,       x1x2,      w1w2,                  z1z2]
    // mul = _mm_add_ps(mul, shuffle);                                         // mul      = [ w1w2+y1y2,  z1z2+x1x2, y1y2+w1w2,             x1x2+z1z2]
    // shuffle = tmath_permute_ps(mul, _MM_SHUFFLE(3, 3, 3, 3));               // shuffle  = [         N,          N,         N,             w1w2+y1y2]
    // mul = _mm_add_ps(shuffle, mul);                                         // mul      = [         N,          N,         N, z1z2+x1x2 + w1w2+y1y2]
    // return tmath_permute_ps(mul, _MM_SHUFFLE(0, 0, 0, 0));                  // splat mul[lane(0)]

    float32_4 t1 = _mm_mul_ps(lhs, rhs);                            // [w1w2, z1z2, y1y2, x1x2]
    float32_4 t2 = _mm_movehl_ps(t1, t1);                           // [?, ?, w1w2, z1z2]
    float32_4 t3 = _mm_add_ps(t1, t2);                              // [?, ?, y1y2 + w1w2, x1x2 + z1z2]
    float32_4 t4 = tmath_permute_ps(t3, _MM_SHUFFLE(1, 1, 1, 1));   // [?, ?, ?, y1y2 + w1w2]
    float32_4 t5 = _mm_add_ss(t3, t4);                              // [?, ?, ?, x1x2 + z1z2 + y1y2 + w1w2]
    return tmath_permute_ps(t5, _MM_SHUFFLE(0, 0, 0, 0));
#endif
}

/**
 * @return (x1y2 - x2y1, same...)
 */
inline float32_4 TMATH_SIMD_CALL_CONV cross2(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    float value = TMATH_NAMESPACE_NAME::cross(lhs.data[0], lhs.data[1], rhs.data[0], rhs.data[1]);
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
    float value = TMATH_NAMESPACE_NAME::cross(lhs.data[0], lhs.data[1], rhs.data[0], rhs.data[1]);
    return { value, value, value, value };
#elif defined(TMATH_USE_SSE2)
    return _mm_set1_ps(0.0f); // TODO cross3
#endif
}

TMATH_SIMD_NAMESPACE_END
