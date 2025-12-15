#pragma once

#include <cmath>

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

// ============================================= operators =============================================
template<is_vector3 TVec3>
bool operator==(const TVec3& lhs, const TVec3& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

template<is_vector3 TVec3>
bool operator!=(const TVec3& lhs, const TVec3& rhs)
{
    return !(lhs == rhs);
}

template<is_vector3 TVec3>
TVec3& operator+=(TVec3& lhs, const TVec3& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

template<is_vector3 TVec3>
TVec3& operator-=(TVec3& lhs, const TVec3& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

template<is_vector3 TVec3>
TVec3& operator*=(TVec3& lhs, const vector_field_t<TVec3> rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

template<is_vector3 TVec3>
TVec3& operator/=(TVec3& lhs, const vector_field_t<TVec3> rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    return lhs;
}

template<is_vector3 TVec3>
void safe_divide_inplace(TVec3& v, const vector_field_t<TVec3> divisor, const TVec3& fallback)
{
    if (is_invalid_divisor(divisor))
    {
        v = fallback;
        return;
    }

    v /= divisor;
}

template<is_vector3 TVec3>
TVec3 operator+(const TVec3& lhs, const TVec3& rhs)
{
    TVec3 v = lhs;
    v += rhs;
    return v;
}

template<is_vector3 TVec3>
TVec3 operator-(const TVec3& lhs, const TVec3& rhs)
{
    TVec3 v = lhs;
    v -= rhs;
    return v;
}

template<is_vector3 TVec3>
TVec3 operator*(const TVec3& lhs, const vector_field_t<TVec3> rhs)
{
    TVec3 v = lhs;
    v *= rhs;
    return v;
}

template<is_vector3 TVec3>
TVec3 operator/(const TVec3& lhs, const vector_field_t<TVec3> rhs)
{
    TVec3 v = lhs;
    v /= rhs;
    return v;
}

template<is_vector3 TVec3>
TVec3 safe_divide(const TVec3& v, const vector_field_t<TVec3> divisor, const TVec3& fallback)
{
    TVec3 result = v;
    safe_divide_inplace(result, divisor, fallback);
    return result;
}

template<is_vector3 TVec3>
vector_field_t<TVec3> dot(const TVec3& lhs, const TVec3& rhs)
{
    return  lhs.x * rhs.x +
            lhs.y * rhs.y +
            lhs.z * rhs.z;
}

template<is_vector3 TVec3>
TVec3 cross(const TVec3& lhs, const TVec3& rhs)
{
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}



// ============================================= casts =============================================
template<is_vector3 Ret, is_vector3 In>
Ret precision_cast(const In& v)
{
    using F = vector_field_t<Ret>;
    return { static_cast<F>(v.x), static_cast<F>(v.y), static_cast<F>(v.z) };
}



// ============================================= functions =============================================
template<is_vector3_floating_point TVec3>
TVec3 to_degrees(const TVec3& radians)
{
    return { to_degrees(radians.x), to_degrees(radians.y), to_degrees(radians.z) };
}

template<is_vector3_floating_point TVec3>
TVec3 to_radians(const TVec3& degrees)
{
    return { to_radians(degrees.x), to_radians(degrees.y), to_radians(degrees.z) };
}

template<is_vector3 TVec3>
TVec3 abs(const TVec3& v)
{
    return { TMATH_NAMESPACE_NAME::abs(v.x), TMATH_NAMESPACE_NAME::abs(v.y), TMATH_NAMESPACE_NAME::abs(v.z) };
}

template<is_vector3_floating_point TVec3>
bool approximately(const TVec3& a, const TVec3& b, const vector_field_t<TVec3> tolerance = MinTolerance<vector_field_t<TVec3>>)
{
    return  approximately(a.x, b.x, tolerance) &&
            approximately(a.y, b.y, tolerance) &&
            approximately(a.z, b.z, tolerance);
}

template<is_floating_point F>
F magnitude(const F x, const F y, const F z)
{
    return std::hypot(x, y, z);
}

template<is_vector3_floating_point TVec3>
vector_field_t<TVec3> magnitude(const TVec3& v)
{
    return std::hypot(v.x, v.y, v.z);
}

template<is_vector3_floating_point TVec3>
void normalize_inplace(TVec3& v)
{
    using F = vector_field_t<TVec3>;
    const F inv_mag = static_cast<F>(1) / magnitude(v);
    v.x *= inv_mag;
    v.y *= inv_mag;
    v.z *= inv_mag;
}

template<is_vector3_floating_point TVec3>
void safe_normalize_inplace(TVec3& v, const TVec3& fallback)
{
    using F = vector_field_t<TVec3>;
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
}

template<is_vector3_floating_point TVec3>
TVec3 normalized(const TVec3& v)
{
    TVec3 result = v;
    TMATH_NAMESPACE_NAME::normalize_inplace(result);
    return result;
}

template<is_vector3_floating_point RetVec3, is_vector3_sint TVec3Int>
RetVec3 normalized(const TVec3Int& v)
{
    using F = vector_field_t<RetVec3>;
    const F inv_mag = static_cast<F>(1) / magnitude(static_cast<F>(v.x), static_cast<F>(v.y), static_cast<F>(v.z));

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag),
        static_cast<F>(v.z * inv_mag)
    };
}

template<is_vector3_floating_point TVec3>
TVec3 safe_normalized(const TVec3& v, const TVec3& fallback)
{
    using F = vector_field_t<TVec3>;
    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag),
        static_cast<F>(v.z * inv_mag)
    };
}

template<is_vector3_floating_point RetVec3, is_vector3_sint TVec3Int>
RetVec3 safe_normalized(const TVec3Int& v, const RetVec3& fallback)
{
    using F = vector_field_t<RetVec3>;
    const F mag = magnitude(static_cast<F>(v.x), static_cast<F>(v.y), static_cast<F>(v.z));
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag),
        static_cast<F>(v.z * inv_mag)
    };
}

template<is_vector3_floating_point TVec3>
vector_field_t<TVec3> distance(const TVec3& a, const TVec3& b)
{
    return TMATH_NAMESPACE_NAME::magnitude(a - b);
}

template<is_vector3_sint TVec3Int>
sint_to_floating_point_t<vector_field_t<TVec3Int>> distance(const TVec3Int& a, const TVec3Int& b)
{
    using F = sint_to_floating_point_t<vector_field_t<TVec3Int>>;
    const TVec3Int delta = a - b;
    return TMATH_NAMESPACE_NAME::magnitude(static_cast<F>(delta.x), static_cast<F>(delta.y), static_cast<F>(delta.z));
}

template<is_vector3_floating_point TVec3, is_floating_point F>
TVec3 lerp(const TVec3& a, const TVec3& b, const F t)
{
    return {
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t),
        lerp(a.z, b.z, t)
    };
}

TMATH_NAMESPACE_END