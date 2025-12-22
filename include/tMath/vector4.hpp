#pragma once

#include <cmath>

#include "math_defs.hpp"
#include "vector1.hpp"

TMATH_CLANG_DIAGNOSTIC_PUSH
TMATH_IGNORE_CLANG_SUBOBJECT_BRACES_WARNING

TMATH_NAMESPACE_BEGIN

// ============================================= operators =============================================
template<is_vector4 TVec4>
constexpr bool operator==(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return  (lhs.x == rhs.x) &&
            (lhs.y == rhs.y) &&
            (lhs.z == rhs.z) &&
            (lhs.w == rhs.w);
}

template<is_vector4 TVec4>
constexpr TVec4& operator+=(TVec4& lhs, const TVec4& rhs) noexcept
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}

template<is_vector4 TVec4>
constexpr TVec4& operator-=(TVec4& lhs, const TVec4& rhs) noexcept
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;
    return lhs;
}

template<is_vector4 TVec4, is_signed_number N>
constexpr TVec4& operator*=(TVec4& lhs, const N rhs) noexcept
{
    using Field = vector_quat_field_t<TVec4>;

    lhs.x *= static_cast<Field>(rhs);
    lhs.y *= static_cast<Field>(rhs);
    lhs.z *= static_cast<Field>(rhs);
    lhs.w *= static_cast<Field>(rhs);
    return lhs;
}

template<is_vector4 TVec4>
constexpr TVec4& operator/=(TVec4& lhs, const vector_quat_field_t<TVec4> rhs) noexcept
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    lhs.w /= rhs;
    return lhs;
}

TMATH_NAMESPACE_END
#include "impl/vector_operators.inl"
TMATH_NAMESPACE_BEGIN


// ============================================= casts =============================================
template<is_vector4 Ret, is_vector4 In>
constexpr Ret vector_cast(const In& v) noexcept
{
    using F = vector_quat_field_t<Ret>;
    return { static_cast<F>(v.x), static_cast<F>(v.y), static_cast<F>(v.z), static_cast<F>(v.w) };
}



// ============================================= functions =============================================
template<is_vector4_floating_point TVec4>
constexpr TVec4 to_degrees(const TVec4& radians) noexcept
{
    return { to_degrees(radians.x), to_degrees(radians.y), to_degrees(radians.z), to_degrees(radians.w) };
}

template<is_vector4_floating_point TVec4>
constexpr TVec4 to_radians(const TVec4& degrees) noexcept
{
    return { to_radians(degrees.x), to_radians(degrees.y), to_radians(degrees.z), to_radians(degrees.w) };
}

template<is_vector4 TVec4>
TVec4 abs(const TVec4& v) noexcept
{
    return { TMATH_NAMESPACE_NAME::abs(v.x), TMATH_NAMESPACE_NAME::abs(v.y), TMATH_NAMESPACE_NAME::abs(v.z), TMATH_NAMESPACE_NAME::abs(v.w) };
}

template<is_vector4 TVec4>
TVec4 sin(const TVec4& v) noexcept
{
    return {
        TMATH_NAMESPACE_NAME::sin(v.x),
        TMATH_NAMESPACE_NAME::sin(v.y),
        TMATH_NAMESPACE_NAME::sin(v.z),
        TMATH_NAMESPACE_NAME::sin(v.w)
    };
}

template<is_vector4 TVec4>
TVec4 cos(const TVec4& v) noexcept
{
    return {
        TMATH_NAMESPACE_NAME::cos(v.x),
        TMATH_NAMESPACE_NAME::cos(v.y),
        TMATH_NAMESPACE_NAME::cos(v.z),
        TMATH_NAMESPACE_NAME::cos(v.w)
    };
}

template<is_vector4 TVec4>
TVec4 tan(const TVec4& v) noexcept
{
    return {
        TMATH_NAMESPACE_NAME::tan(v.x),
        TMATH_NAMESPACE_NAME::tan(v.y),
        TMATH_NAMESPACE_NAME::tan(v.z),
        TMATH_NAMESPACE_NAME::tan(v.w)
    };
}

template<is_vector4 TVec4>
constexpr TVec4 hadamard_mul(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z,
        lhs.w * rhs.w
    };
}

template<is_vector4 TVec4>
constexpr TVec4 hadamard_div(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z,
        lhs.w / rhs.w
    };
}

template<is_floating_point F>
constexpr F dot(
    const F x1, const F y1, const F z1, const F w1,
    const F x2, const F y2, const F z2, const F w2
) noexcept
{
    return  x1 * x2 +
            y1 * y2 +
            z1 * z2 +
            w1 * w2;
}

template<is_vector4 TVec4>
constexpr vector_quat_field_t<TVec4> dot(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return  lhs.x * rhs.x +
            lhs.y * rhs.y +
            lhs.z * rhs.z +
            lhs.w * rhs.w;
}

// vector3's cross, ignore w component, the return value's w is zero
template<is_vector4 TVec4>
constexpr TVec4 cross(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
        static_cast<vector_quat_field_t<TVec4>>(0)
    };
}

template<is_vector4_floating_point TVec4>
bool approximately(const TVec4& a, const TVec4& b, const vector_quat_field_t<TVec4> tolerance = MinTolerance<vector_quat_field_t<TVec4>>) noexcept
{
    return  approximately(a.x, b.x, tolerance) &&
            approximately(a.y, b.y, tolerance) &&
            approximately(a.z, b.z, tolerance) &&
            approximately(a.w, b.w, tolerance);
}

template<is_floating_point F>
F magnitude(const F x, const F y, const F z, const F w) noexcept
{
    return std::sqrt(
        x * x +
        y * y +
        z * z +
        w * w
    );
}

template<is_vector4_floating_point TVec4>
vector_quat_field_t<TVec4> magnitude(const TVec4& v) noexcept
{
    return std::sqrt(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z +
        v.w * v.w
    );
}

template<is_vector4_floating_point TVec4>
void normalize_inplace(TVec4& v) noexcept
{
    using F = vector_quat_field_t<TVec4>;
    const F inv_mag = static_cast<F>(1) / magnitude(v);
    v.x *= inv_mag;
    v.y *= inv_mag;
    v.z *= inv_mag;
    v.w *= inv_mag;
}

template<is_vector4_floating_point TVec4>
void safe_normalize_inplace(TVec4& v, const TVec4& fallback) noexcept
{
    using F = vector_quat_field_t<TVec4>;
    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        v = fallback;
        return;
    }

    const F inv_mag = static_cast<F>(1) / mag;
    v.x *= inv_mag;
    v.y *= inv_mag;
    v.z *= inv_mag;
    v.w *= inv_mag;
}

template<is_vector4_floating_point TVec4>
TVec4 normalized(const TVec4& v) noexcept
{
    TVec4 result = v;
    normalize_inplace(result);
    return result;
}

template<is_vector4_floating_point RetVec3, is_vector4_sint TVec4Int>
RetVec3 normalized(const TVec4Int& v) noexcept
{
    using F = vector_quat_field_t<RetVec3>;
    const F inv_mag = static_cast<F>(1) / magnitude(static_cast<F>(v.x), static_cast<F>(v.y), static_cast<F>(v.z), static_cast<F>(v.w));

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag),
        static_cast<F>(v.z * inv_mag),
        static_cast<F>(v.w * inv_mag)
    };
}

template<is_vector4_floating_point TVec4>
TVec4 safe_normalized(const TVec4& v, const TVec4& fallback) noexcept
{
    using F = vector_quat_field_t<TVec4>;
    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag),
        static_cast<F>(v.z * inv_mag),
        static_cast<F>(v.w * inv_mag)
    };
}

template<is_vector4_floating_point RetVec3, is_vector4_sint TVec4Int>
RetVec3 safe_normalized(const TVec4Int& v, const RetVec3& fallback) noexcept
{
    using F = vector_quat_field_t<RetVec3>;
    const F mag = magnitude(static_cast<F>(v.x), static_cast<F>(v.y), static_cast<F>(v.z), static_cast<F>(v.w));
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag),
        static_cast<F>(v.z * inv_mag),
        static_cast<F>(v.w * inv_mag)
    };
}

template<is_vector4_floating_point TVec4>
vector_quat_field_t<TVec4> distance(const TVec4& a, const TVec4& b) noexcept
{
    return magnitude(a - b);
}

template<is_vector4_sint TVec4Int>
sint_to_floating_point_t<vector_quat_field_t<TVec4Int>> distance(const TVec4Int& a, const TVec4Int& b) noexcept
{
    using F = sint_to_floating_point_t<vector_quat_field_t<TVec4Int>>;
    const TVec4Int delta = a - b;
    return magnitude(static_cast<F>(delta.x), static_cast<F>(delta.y), static_cast<F>(delta.z), static_cast<F>(delta.w));
}

template<is_vector4_floating_point TVec4, is_floating_point F>
constexpr TVec4 lerp(const TVec4& a, const TVec4& b, const F t) noexcept
{
    return {
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t),
        lerp(a.z, b.z, t),
        lerp(a.w, b.w, t)
    };
}


TMATH_NAMESPACE_END

TMATH_CLANG_DIAGNOSTIC_POP
