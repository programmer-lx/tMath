#pragma once

#include <xmmintrin.h>

#include "../math_defs.hpp"

#define TMATH_SIMD_NAMESPACE_NAME tSimd
#define TMATH_SIMD_NAMESPACE_BEGIN namespace TMATH_SIMD_NAMESPACE_NAME {
#define TMATH_SIMD_NAMESPACE_END }



#define TMATH_SIMD_VECTOR_OPERATORS(vector_type_name) \
    TMATH_FORCE_INLINE friend vector_type_name& operator+=(vector_type_name& lhs, vector_type_name rhs) \
    { return TMATH_SIMD_NAMESPACE_NAME::operator+=(lhs, rhs); } \
    TMATH_FORCE_INLINE friend vector_type_name& operator-=(vector_type_name& lhs, vector_type_name rhs) \
    { return TMATH_SIMD_NAMESPACE_NAME::operator-=(lhs, rhs); }


#define TMATH_SIMD_FLOAT4(vector_type_name) \
    union \
    { \
        __m128 mem; \
        struct { float x, y, z, w; }; \
        struct { float r, g, b, a; }; \
        float data[4]; \
    }; \
    TMATH_SIMD_VECTOR_OPERATORS(vector_type_name) \
    explicit vector_type_name(__m128 v) noexcept : mem(v) {} \
    explicit vector_type_name(float _x, float _y, float _z, float _w) noexcept { mem = _mm_set_ps(_w, _z, _y, _x); } \
    template<TMATH_NAMESPACE_NAME::is_vector4_float TVec4> \
    static vector_type_name load_from(const TVec4& vec) { return vector_type_name(_mm_load_ps(vec.data)); } \
    template<TMATH_NAMESPACE_NAME::is_vector4_float TVec4> \
    void store_to(TVec4& vec) { _mm_store_ps(vec.data, mem); }




TMATH_SIMD_NAMESPACE_BEGIN

template<typename V>
concept is_simd_float32_4 =
requires(V t, float f1, float f2, float f3, float f4)
{
    // 从四个 float 构造
    { V(f1, f2, f3, f4) } -> std::same_as<V>;
} &&
requires(V v)
{
    // 能够直接从__m128构造
    requires std::is_nothrow_constructible_v<V, __m128>;

    v.mem;
    requires std::is_same_v<decltype(v.mem), __m128>;

    v.data;
    requires std::is_same_v<std::decay_t<decltype(v.data[0])>, float>;
    requires std::convertible_to<decltype(v.data), const float*>;

    v.x;
    v.y;
    v.z;
    v.w;
    requires std::is_same_v<decltype(v.x), float>;
    requires std::is_same_v<decltype(v.y), float>;
    requires std::is_same_v<decltype(v.z), float>;
    requires std::is_same_v<decltype(v.w), float>;

    requires std::is_standard_layout_v<V>;
    requires std::is_trivially_copyable_v<V>;

} && (alignof(V) == alignof(__m128)) && (sizeof(V) == sizeof(__m128));



TMATH_SIMD_NAMESPACE_END
