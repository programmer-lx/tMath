#pragma once

#include "types.hpp"

TMATH_SIMD_NAMESPACE_BEGIN

// fn: permute
#if defined(TMATH_SIMD_USE_AVX)
    #define tmath_permute_ps(v, imm8) _mm_permute_ps((v), (imm8))
#else
    #define tmath_permute_ps(v, imm8) _mm_shuffle_ps((v), (v), (imm8))
#endif

// fn: fmadd
#if defined(TMATH_SIMD_USE_FMA3)
    #define tmath_fmadd_ps(a, b, c) _mm_fmadd_ps((a), (b), (c))
#else
    #define tmath_fmadd_ps(a, b, c) _mm_add_ps(_mm_mul_ps((a), (b)), (c))
#endif

// fn: a or b's component is nan
#define tmath_cmp_nan_any_ps(a, b) _mm_cmpunord_ps((a), (b))

// fn: x's component is nan
#define tmath_is_nan_any_ps(x) tmath_cmp_nan_any_ps((x), (x))

// ========================================= load store get set =========================================

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

inline float32_4 TMATH_SIMD_CALL_CONV load(float32 scalar) noexcept
{
#if defined(TMATH_NO_SIMD)
    return { scalar, scalar, scalar, scalar };
#else
    return _mm_set1_ps(scalar);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV load(float32 x, float32 y, float32 z, float32 w) noexcept
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
    alignas(TMATH_SIMD_128_ALIGNMENT) float32 tmp[4];
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
    alignas(TMATH_SIMD_128_ALIGNMENT) float32 tmp[4];
    _mm_store_ps(tmp, v);
    vec.data[0] = tmp[0];
    vec.data[1] = tmp[1];
#endif
}

inline float32 TMATH_SIMD_CALL_CONV get_x(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.data[0];
#else
    return _mm_cvtss_f32(v);
#endif
}

inline float32 TMATH_SIMD_CALL_CONV get_y(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.data[1];
#else
    return _mm_cvtss_f32(tmath_permute_ps(v, _MM_SHUFFLE(1, 1, 1, 1)));
#endif
}

inline float32 TMATH_SIMD_CALL_CONV get_z(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.data[2];
#else
    return _mm_cvtss_f32(tmath_permute_ps(v, _MM_SHUFFLE(2, 2, 2, 2)));
#endif
}

inline float32 TMATH_SIMD_CALL_CONV get_w(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return v.data[3];
#else
    return _mm_cvtss_f32(tmath_permute_ps(v, _MM_SHUFFLE(3, 3, 3, 3)));
#endif
}

// ========================================= 基本数学运算 =========================================

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
 * 如果x的某个分量是NaN，则把那个分量置为1-bits，否则置为0-bits
 */
inline float32_4 TMATH_SIMD_CALL_CONV is_nan(float32_4_arg_in x) noexcept
{
#if defined(TMATH_NO_SIMD)
    return {
        std::bit_cast<float32>(TMATH_NAMESPACE_NAME::is_nan(x.data[0]) ? 0xffffffffu : 0x0u),
        std::bit_cast<float32>(TMATH_NAMESPACE_NAME::is_nan(x.data[1]) ? 0xffffffffu : 0x0u),
        std::bit_cast<float32>(TMATH_NAMESPACE_NAME::is_nan(x.data[2]) ? 0xffffffffu : 0x0u),
        std::bit_cast<float32>(TMATH_NAMESPACE_NAME::is_nan(x.data[3]) ? 0xffffffffu : 0x0u)
    };
#else
    return tmath_is_nan_any_ps(x);
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

inline float32_4 TMATH_SIMD_CALL_CONV mul(float32_4_arg_in lhs, float32 scalar) noexcept
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
    return tmath::cwise_div(lhs, rhs);
#else
    return _mm_div_ps(lhs, rhs);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV div(float32_4_arg_in lhs, float32 scalar) noexcept
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
    return tmath::cwise_mul(a, b) + c;
#else
    return tmath_fmadd_ps(a, b, c);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV abs(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::abs(v);
#else
    return _mm_and_ps(v, detail128::Abs4.f32_4);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV sqrt(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return {
        TMATH_NAMESPACE_NAME::sqrt(v.data[0]),
        TMATH_NAMESPACE_NAME::sqrt(v.data[1]),
        TMATH_NAMESPACE_NAME::sqrt(v.data[2]),
        TMATH_NAMESPACE_NAME::sqrt(v.data[3])
    };
#else
    return _mm_sqrt_ps(v);
#endif
}

inline float32_4 TMATH_SIMD_CALL_CONV sin(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::sin(v);
#elif defined(TMATH_SIMD_USE_SVML)
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
#elif defined(TMATH_SIMD_USE_SVML)
    return _mm_cos_ps(v);
#else
    // TODO cos
    return _mm_set1_ps(0.0f);
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


// ========================================= 其他函数 =========================================

/**
 * 测试a和b的各个分量是否近似相等
 * @return 如果该分量近似相等，则该分量不为0，否则为0
 * @warning 返回的分量只能保证是否为0，所以只能通过是否为0进行判断
 */
inline float32_4 TMATH_SIMD_CALL_CONV approximately_cwise(float32_4_arg_in a, float32_4_arg_in b, float32 tolerance) noexcept
{
    // 对于单个分量: c = ( abs(a - b) <= tolerance ) ? (非0) : 0

#if defined(TMATH_NO_SIMD)
    return TMATH_NAMESPACE_NAME::approximately_cwise(a, b, tolerance);
#else
    float32_4 tolerance_4 = _mm_set1_ps(tolerance);
    float32_4 diff = _mm_sub_ps(a, b);
    float32_4 abs_diff = _mm_and_ps(diff, detail128::Abs4.f32_4);
    return _mm_cmple_ps(abs_diff, tolerance_4);
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

inline float32_4 TMATH_SIMD_CALL_CONV lerp(float32_4_arg_in a, float32_4_arg_in b, float32 t) noexcept
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

inline float32_4 TMATH_SIMD_CALL_CONV dot2(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    float32 value = TMATH_NAMESPACE_NAME::dot(lhs.data[0], lhs.data[1], rhs.data[0], rhs.data[1]);
    return { value, value, value, value };
#elif defined(TMATH_SIMD_USE_SSE4_1)
    return _mm_dp_ps(lhs, rhs, 0x3f);                   // imm8: 7-4: compute, 3-0: store
#elif defined(TMATH_SIMD_USE_SSE3)
    float32_4 temp = _mm_mul_ps(lhs, rhs);              // temp = [             w1w2,             z1z2,             y1y2,             x1x2]
    temp = _mm_and_ps(temp, detail128::Lane01.f32_4);   // temp = [                0,             z1z2,             y1y2,             x1x2]
    temp = _mm_hadd_ps(temp, temp);                     // temp = [             z1z2,        x1x2+y1y2,             z1z2,        x1x2+y1y2]
    return _mm_hadd_ps(temp, temp);                     // temp = [ x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2]
#else
    float32_4 mul = _mm_mul_ps(lhs, rhs);                                   // mul      = [     w1w2,      z1z2, y1y2,                  x1x2]
    mul = _mm_and_ps(mul, detail128::Lane01.f32_4);                         // mul      = [        0,         0, y1y2,                  x1x2]
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
    float32 value = TMATH_NAMESPACE_NAME::dot(lhs.data[0], lhs.data[1], lhs.data[2], rhs.data[0], rhs.data[1], rhs.data[2]);
    return { value, value, value, value };
#elif defined(TMATH_SIMD_USE_SSE4_1)
    return _mm_dp_ps(lhs, rhs, 0x7f);                   // imm8: 7-4: compute, 3-0: store
#elif defined(TMATH_SIMD_USE_SSE3)
    float32_4 temp = _mm_mul_ps(lhs, rhs);              // temp = [             w1w2,             z1z2,             y1y2,             x1x2]
    temp = _mm_and_ps(temp, detail128::Lane012.f32_4);  // temp = [                0,             z1z2,             y1y2,             x1x2]
    temp = _mm_hadd_ps(temp, temp);                     // temp = [             z1z2,        x1x2+y1y2,             z1z2,        x1x2+y1y2]
    return _mm_hadd_ps(temp, temp);                     // temp = [ x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2, x1x2+y1y2 + z1z2]
#else
    float32_4 mul = _mm_mul_ps(lhs, rhs);                                   // mul      = [     w1w2,      z1z2, y1y2,                  x1x2]
    mul = _mm_and_ps(mul, detail128::Lane012.f32_4);                        // mul      = [        0,      z1z2, y1y2,                  x1x2]
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
    float32 value = TMATH_NAMESPACE_NAME::dot(lhs, rhs);
    return { value, value, value, value };
#elif defined(TMATH_SIMD_USE_SSE4_1)
    return _mm_dp_ps(lhs, rhs, 0xff);
#elif defined(TMATH_SIMD_USE_SSE3)
    float32_4 t1 = _mm_mul_ps(lhs, rhs);  // t1 = [                 w1w2,                  z1z2,                  y1y2,                  x1x2]
    float32_4 t2 = _mm_hadd_ps(t1, t1);   // t2 = [            z1z2+w1w2,             x1x2+y1y2,             z1z2+w1w2,             x1x2+y1y2]
    return _mm_hadd_ps(t2, t2);           // re = [x1x2+y1y2 + z1z2+w1w2, x1x2+y1y2 + z1z2+w1w2, x1x2+y1y2 + z1z2+w1w2, x1x2+y1y2 + z1z2+w1w2]
#else
    float32_4 t1 = _mm_mul_ps(lhs, rhs);                            // [w1w2, z1z2, y1y2, x1x2]
    float32_4 t2 = _mm_movehl_ps(t1, t1);                           // [?, ?, w1w2, z1z2]
    float32_4 t3 = _mm_add_ps(t1, t2);                              // [?, ?, y1y2 + w1w2, x1x2 + z1z2]
    float32_4 t4 = tmath_permute_ps(t3, _MM_SHUFFLE(1, 1, 1, 1));   // [?, ?, ?, y1y2 + w1w2]
    float32_4 t5 = _mm_add_ss(t3, t4);                              // [?, ?, ?, x1x2 + z1z2 + y1y2 + w1w2]
    return tmath_permute_ps(t5, _MM_SHUFFLE(0, 0, 0, 0));
#endif
}

/**
 * 2D向量叉乘(用于判断两个向量的顺逆时针关系)
 * @return (x1y2 - x2y1, same...)
 */
inline float32_4 TMATH_SIMD_CALL_CONV cross2(float32_4_arg_in lhs, float32_4_arg_in rhs) noexcept
{
#if defined(TMATH_NO_SIMD)
    float32 value = TMATH_NAMESPACE_NAME::cross(lhs.data[0], lhs.data[1], rhs.data[0], rhs.data[1]);
    return { value, value, value, value };
#else
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
    float32 value = TMATH_NAMESPACE_NAME::cross(lhs.data[0], lhs.data[1], rhs.data[0], rhs.data[1]);
    return { value, value, value, value };
#else
    return _mm_set1_ps(0.0f); // TODO cross3
#endif
}

/**
 * 2D向量模长
 */
inline float32_4 TMATH_SIMD_CALL_CONV magnitude2(float32_4_arg_in v) noexcept
{
    // mag = sqrt(x^2 + y^2)

#if defined(TMATH_NO_SIMD)
    const float32 m = TMATH_NAMESPACE_NAME::magnitude(v.data[0], v.data[1]);
    return {
        m, m, m, m
    };
#else
    float32_4 t1 = _mm_mul_ps(v, v);                                // [w^2, z^2, y^2, x^2]
    float32_4 t2 = tmath_permute_ps(t1, _MM_SHUFFLE(0, 1, 0, 1));   // [x^2, y^2, x^2, y^2]
    float32_4 t3 = _mm_add_ss(t1, t2);                              // [?, ?, ?, x^2 + y^2]
    float32_4 t4 = tmath_permute_ps(t3, _MM_SHUFFLE(0, 0, 0, 0));   // [x^2 + y^2, ...]
    return _mm_sqrt_ps(t4);
#endif
}

/**
 * 3D向量模长
 */
inline float32_4 TMATH_SIMD_CALL_CONV magnitude3(float32_4_arg_in v) noexcept
{
    // mag = sqrt(x^2 + y^2 + z^2)

#if defined(TMATH_NO_SIMD)
    const float32 m = TMATH_NAMESPACE_NAME::magnitude(v.data[0], v.data[1], v.data[2]);
    return {
        m, m, m, m
    };
#else
    // 先计算 x^2 + y^2
    // 然后再与 z^2 直接相加求和
    // 最后开平方
    float32_4 t1 = _mm_mul_ps(v, v);                                // [w^2, z^2, y^2, x^2]
    float32_4 t2 = tmath_permute_ps(t1, _MM_SHUFFLE(2, 0, 3, 1));   // [z^2, x^2, z^2, y^2]
    float32_4 t3 = _mm_add_ss(t1, t2);                              // [?, ?, ?, x^2 + y^2]
    float32_4 t4 = tmath_permute_ps(t1, _MM_SHUFFLE(2, 2, 2, 2));   // [...................... z^2]
    float32_4 t5 = _mm_add_ss(t3, t4);                              // [?, ?, ?, x^2 + y^2 + z^2]
    return _mm_sqrt_ps(tmath_permute_ps(t5, _MM_SHUFFLE(0, 0, 0, 0)));
#endif
}

/**
 * 4D向量模长
 */
inline float32_4 TMATH_SIMD_CALL_CONV magnitude4(float32_4_arg_in v) noexcept
{
    // mag = sqrt(x^2 + y^2 + z^2 + w^2)

#if defined(TMATH_NO_SIMD)
    const float32 m = TMATH_NAMESPACE_NAME::magnitude(v.data[0], v.data[1], v.data[2], v.data[3]);
    return {
        m, m, m, m
    };
#else
    // 先计算 x^2 + y^2
    // 再计算 z^2 + w^2
    // 然后直接相加求和
    // 最后开平方
    float32_4 t1 = _mm_mul_ps(v, v);                                // [w^2, z^2, y^2, x^2]
    float32_4 t2 = tmath_permute_ps(t1, _MM_SHUFFLE(2, 3, 0, 1));   // [z^2, w^2, x^2, y^2]
    float32_4 t3 = _mm_add_ps(t1, t2);                              // [z^2 + w^2, z^2 + w^2, x^2 + y^2, x^2 + y^2]
    float32_4 t4 = tmath_permute_ps(t3, _MM_SHUFFLE(1, 0, 3, 2));   // [x^2 + y^2, x^2 + y^2, z^2 + w^2, z^2 + w^2]
    float32_4 t5 = _mm_add_ps(t3, t4);                              // [x^2 + y^2 + z^2 + w^2, ...]
    return _mm_sqrt_ps(t5);
#endif
}

/**
 * 2D向量模长的平方
 */
inline float32_4 TMATH_SIMD_CALL_CONV magnitude2_sq(float32_4_arg_in v) noexcept
{
    return dot2(v, v);
}

/**
 * 3D向量模长的平方
 */
inline float32_4 TMATH_SIMD_CALL_CONV magnitude3_sq(float32_4_arg_in v) noexcept
{
    return dot3(v, v);
}

/**
 * 4D向量模长的平方
 */
inline float32_4 TMATH_SIMD_CALL_CONV magnitude4_sq(float32_4_arg_in v) noexcept
{
    return dot4(v, v);
}

/**
 * 2D向量归一化
 * @warning 计算过程中如果遇到inf, nan，则返回 (nan, nan, nan, nan)
 */
inline float32_4 TMATH_SIMD_CALL_CONV normalize2(float32_4_arg_in v) noexcept
{
    // magnitude = sqrt(x^2 + y^2)
    // result = v / magnitude

#if defined(TMATH_NO_SIMD)
    const float32 mag = TMATH_NAMESPACE_NAME::magnitude(v.data[0], v.data[1]);
    const float32 inv_mag = (mag != 0.0f) ? (1.0f / mag) : 0.0f;

    const uint32_t is_not_inf_mask = TMATH_NAMESPACE_NAME::is_infinity(mag) ? 0x0 : 0xffffffff;
    const uint32_t nan_bits = std::bit_cast<uint32_t>(TMATH_NAMESPACE_NAME::QuietNaN<float>) & (~is_not_inf_mask);

    return {
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[0] * inv_mag) & is_not_inf_mask) | nan_bits ),
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[1] * inv_mag) & is_not_inf_mask) | nan_bits ),
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[2] * inv_mag) & is_not_inf_mask) | nan_bits ),
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[3] * inv_mag) & is_not_inf_mask) | nan_bits )
    };
#else
    float32_4 sq = _mm_mul_ps(v, v);                                    // [?, ?,     y^2, x^2]
    float32_4 t1 = tmath_permute_ps(sq, _MM_SHUFFLE(1, 1, 1, 1));       // [?, ?, ?,       y^2]
    sq = _mm_add_ss(sq, t1);                                            // [?, ?, ?, x^2 + y^2]
    sq = tmath_permute_ps(sq, _MM_SHUFFLE(0, 0, 0, 0));                 // [x^2 + y^2, .......]
    float32_4 mag = _mm_sqrt_ps(sq);                                  // [mag, .............]

    float32_4 not_zero_mask = _mm_cmpneq_ps(mag, _mm_setzero_ps());                 // 如果mag == 0，zero_mark = 0, 否则 = 0xffffffff
    float32_4 mag_is_not_inf_mask = _mm_cmpneq_ps(mag, detail128::Inf.f32_4);       // 如果mag不是inf(长度正常)，则把他设置为0xffffffff

    float32_4 result = _mm_div_ps(v, mag);
    result = _mm_and_ps(result, not_zero_mask); // 如果 mag == 0，result赋值为0向量

    // 如果mag == inf，则返回 nan 向量
    float32_4 t2 = _mm_andnot_ps(mag_is_not_inf_mask, detail128::QuietNaN.f32_4);   // t2 = (mag == inf) ? nan : 0
    float32_4 t3 = _mm_and_ps(result, mag_is_not_inf_mask);                         // t3 = (mag == inf) ?   0 : result
    result = _mm_or_ps(t2, t3);
    return result;
#endif
}

/**
 * 3D向量归一化
 * @warning 计算过程中如果遇到inf, nan，则返回 (nan, nan, nan, nan)
 */
inline float32_4 TMATH_SIMD_CALL_CONV normalize3(float32_4_arg_in v) noexcept
{
    // magnitude = sqrt(x^2 + y^2 + z^2)
    // result = v / magnitude

#if defined(TMATH_NO_SIMD)
    const float32 mag = TMATH_NAMESPACE_NAME::magnitude(v.data[0], v.data[1], v.data[2]);
    const float32 inv_mag = (mag > 0.0f) ? (1.0f / mag) : mag;

    const uint32_t is_not_inf_mask = TMATH_NAMESPACE_NAME::is_infinity(mag) ? 0x0 : 0xffffffff;
    const uint32_t nan_bits = std::bit_cast<uint32_t>(TMATH_NAMESPACE_NAME::QuietNaN<float>) & (~is_not_inf_mask);

    return {
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[0] * inv_mag) & is_not_inf_mask) | nan_bits ),
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[1] * inv_mag) & is_not_inf_mask) | nan_bits ),
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[2] * inv_mag) & is_not_inf_mask) | nan_bits ),
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[3] * inv_mag) & is_not_inf_mask) | nan_bits )
    };
#else
    float32_4 sq = _mm_mul_ps(v, v);                                // [?, z^2, y^2, x^2]
    float32_4 t1 = tmath_permute_ps(sq, _MM_SHUFFLE(3, 0, 2, 1));   // [?, x^2, z^2, y^2]
    sq = _mm_add_ss(sq, t1);                                        // [?, ?, ?, x^2 + y^2]
    t1 = tmath_permute_ps(t1, _MM_SHUFFLE(3, 3, 3, 1));             // [?, ?, ?, z^2]
    sq = _mm_add_ss(sq, t1);                                        // [......, x^2 + y^2 + z^2]
    sq = tmath_permute_ps(sq, _MM_SHUFFLE(0, 0, 0, 0));             // [x^2 + y^2 + z^2]

    float32_4 mag = _mm_sqrt_ps(sq);                              // [magnitude.......]
    float32_4 result = _mm_div_ps(v, mag);

    // zero vector
    float32_4 not_zero_mask = _mm_cmpneq_ps(mag, _mm_setzero_ps()); // mask = (mag == 0) ? 0x0 : 0xffffffff
    result = _mm_and_ps(result, not_zero_mask);

    // inf / nan
    // if mag == inf : result = nan

    // 接下来要对结果进行选择：要根据位掩码，选择result | nan
    // 可以使用 or 运算进行选择：构造一个 (result : 0) 和 (0 : nan)
    // 然后再确定一个条件变量，确保那个条件变量只能构造出上面两个对 (并且这个条件变量跟inf有关)
    // 构造出上面的数之后，直接一个 or 运算，就能返回 result 或者 nan

    float32_4 mag_is_not_inf_mask = _mm_cmpneq_ps(mag, detail128::Inf.f32_4); // mask = (mag == inf) ? 0x0 : 0xffffffff
    // (result : 0)
    float32_4 result_or_zero = _mm_and_ps(result, mag_is_not_inf_mask);
    // (0 : nan)
    float32_4 zero_or_nan = _mm_andnot_ps(mag_is_not_inf_mask, detail128::QuietNaN.f32_4);

    result = _mm_or_ps(result_or_zero, zero_or_nan);
    return result;
#endif
}

/**
 * 4D向量归一化
 * @warning 计算过程中如果遇到inf, nan，则返回 (nan, nan, nan, nan)
 */
inline float32_4 TMATH_SIMD_CALL_CONV normalize4(float32_4_arg_in v) noexcept
{
#if defined(TMATH_NO_SIMD)
    const float32 mag = TMATH_NAMESPACE_NAME::magnitude(v.data[0], v.data[1], v.data[2], v.data[3]);
    const float32 inv_mag = (mag > 0.0f) ? (1.0f / mag) : mag;

    const uint32_t is_not_inf_mask = TMATH_NAMESPACE_NAME::is_infinity(mag) ? 0x0 : 0xffffffff;
    const uint32_t nan_bits = std::bit_cast<uint32_t>(TMATH_NAMESPACE_NAME::QuietNaN<float>) & (~is_not_inf_mask);

    return {
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[0] * inv_mag) & is_not_inf_mask) | nan_bits ),
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[1] * inv_mag) & is_not_inf_mask) | nan_bits ),
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[2] * inv_mag) & is_not_inf_mask) | nan_bits ),
        std::bit_cast<float32>( (std::bit_cast<uint32_t>(v.data[3] * inv_mag) & is_not_inf_mask) | nan_bits )
    };
#else
    float32_4 sq = _mm_mul_ps(v, v);                                // [w^2, z^2, y^2, x^2]
    float32_4 t1 = tmath_permute_ps(sq, _MM_SHUFFLE(2, 3, 0, 1));   // [z^2, w^2, x^2, y^2]
    sq = _mm_add_ps(sq, t1);                                        // [zw^2, zw^2, xy^2, xy^2]
    t1 = tmath_permute_ps(sq, _MM_SHUFFLE(1, 0, 3, 2));             // [xy^2, xy^2, zw^2, zw^2]
    sq = _mm_add_ps(sq, t1);                                        // [xyzw^2, ..............]

    float32_4 mag = _mm_sqrt_ps(sq);
    float32_4 result = _mm_div_ps(v, mag);

    // zero vector
    float32_4 t2 = _mm_cmpneq_ps(mag, _mm_setzero_ps());
    result = _mm_and_ps(result, t2);

    // inf / nan
    float32_4 t3 = _mm_cmpneq_ps(mag, detail128::Inf.f32_4); // mask = mag is not inf
    // (result : 0)
    t2 = _mm_and_ps(result, t3);
    // (0 : nan)
    t1 = _mm_andnot_ps(t3, detail128::QuietNaN.f32_4);

    result = _mm_or_ps(t1, t2);
    return result;
#endif
}

TMATH_SIMD_NAMESPACE_END
