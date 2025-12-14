#pragma once

#include <cmath>

#include "math_defs.hpp"
#include "vector1.hpp"

TMATH_NAMESPACE_BEGIN

// ============================================= operators =============================================
template<is_vector2 TVec2>
bool operator==(const TVec2& lhs, const TVec2& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

template<is_vector2 TVec2>
bool operator!=(const TVec2& lhs, const TVec2& rhs)
{
    return !(lhs == rhs);
}

template<is_vector2 TVec2>
TVec2& operator+=(TVec2& lhs, const TVec2& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

template<is_vector2 TVec2>
TVec2& operator-=(TVec2& lhs, const TVec2& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

template<is_vector2 TVec2>
TVec2& operator*=(TVec2& lhs, const vector_field_t<TVec2> rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

template<is_vector2 TVec2>
TVec2& operator/=(TVec2& lhs, const vector_field_t<TVec2> rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    return lhs;
}

template<is_vector2 TVec2>
void safe_divide_inplace(TVec2& val, const vector_field_t<TVec2> divisor, const TVec2& fallback)
{
    if (is_invalid_divisor(divisor))
    {
        val = fallback;
        return;
    }

    val /= divisor;
}

template<is_vector2 TVec2>
TVec2 operator+(const TVec2& lhs, const TVec2& rhs)
{
    TVec2 v = lhs;
    v += rhs;
    return v;
}

template<is_vector2 TVec2>
TVec2 operator-(const TVec2& lhs, const TVec2& rhs)
{
    TVec2 v = lhs;
    v -= rhs;
    return v;
}

template<is_vector2 TVec2>
TVec2 operator*(const TVec2& lhs, const vector_field_t<TVec2> rhs)
{
    TVec2 v = lhs;
    v *= rhs;
    return v;
}

template<is_vector2 TVec2>
TVec2 operator/(const TVec2& lhs, const vector_field_t<TVec2> rhs)
{
    TVec2 v = lhs;
    v /= rhs;
    return v;
}

template<is_vector2 TVec2>
TVec2 safe_divide(const TVec2& val, const vector_field_t<TVec2> divisor, const TVec2& fallback)
{
    TVec2 result = val;
    safe_divide_inplace(result, divisor, fallback);
    return result;
}

template<is_vector2 TVec2>
vector_field_t<TVec2> dot(const TVec2& lhs, const TVec2& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<is_vector2 TVec2>
vector_field_t<TVec2> cross(const TVec2& lhs, const TVec2& rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}



// ============================================= casts =============================================
template<is_vector2 Ret, is_vector2 In>
Ret precision_cast(const In& v)
{
    using F = vector_field_t<Ret>;
    return { static_cast<F>(v.x), static_cast<F>(v.y) };
}



// ============================================= functions =============================================
template<is_vector2_floating_point TVec2>
TVec2 to_degrees(const TVec2& radians)
{
    return { to_degrees(radians.x), to_degrees(radians.y) };
}

template<is_vector2_floating_point TVec2>
TVec2 to_radians(const TVec2& degrees)
{
    return { to_radians(degrees.x), to_radians(degrees.y) };
}

template<is_vector2 TVec2>
TVec2 abs(const TVec2& v)
{
    return { TMATH_NAMESPACE_NAME::abs(v.x), TMATH_NAMESPACE_NAME::abs(v.y) };
}

template<is_vector2_floating_point TVec2>
bool approximately(const TVec2& a, const TVec2& b, const vector_field_t<TVec2> tolerance = MinTolerance<vector_field_t<TVec2>>)
{
    return approximately(a.x, b.x, tolerance) && approximately(a.y, b.y, tolerance);
}

template<is_floating_point F>
F magnitude(const F x, const F y)
{
    return std::hypot(x, y);
}

template<is_vector2_floating_point TVec2>
vector_field_t<TVec2> magnitude(const TVec2& v)
{
    return std::hypot(v.x, v.y);
}

template<is_vector2_floating_point TVec2>
void normalize_inplace(TVec2& v)
{
    using F = vector_field_t<TVec2>;
    const F inv_mag = static_cast<F>(1) / magnitude(v);
    v.x *= inv_mag;
    v.y *= inv_mag;
}

template<is_vector2_floating_point TVec2>
void safe_normalize_inplace(TVec2& v, const TVec2& fallback)
{
    using F = vector_field_t<TVec2>;
    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        v = fallback;
        return;
    }

    const F inv_mag = static_cast<F>(1) / mag;
    v.x *= inv_mag;
    v.y *= inv_mag;
}

template<is_vector2_floating_point TVec2>
TVec2 normalized(const TVec2& v)
{
    TVec2 result = v;
    TMATH_NAMESPACE_NAME::normalize_inplace(result);
    return result;
}

template<is_vector2_floating_point RetVec2, is_vector2_sint TVec2Int>
RetVec2 normalized(const TVec2Int& v)
{
    using F = vector_field_t<RetVec2>;
    const F inv_mag = static_cast<F>(1) / magnitude(static_cast<F>(v.x), static_cast<F>(v.y));

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag)
    };
}

template<is_vector2_floating_point TVec2>
TVec2 safe_normalized(const TVec2& v, const TVec2& fallback)
{
    using F = vector_field_t<TVec2>;
    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag)
    };
}

template<is_vector2_floating_point RetVec2, is_vector2_sint TVec2Int>
RetVec2 safe_normalized(const TVec2Int& v, const RetVec2& fallback)
{
    using F = vector_field_t<RetVec2>;
    const F mag = magnitude(static_cast<F>(v.x), static_cast<F>(v.y));
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag)
    };
}

template<is_vector2_floating_point TVec2>
vector_field_t<TVec2> distance(const TVec2& a, const TVec2& b)
{
    return TMATH_NAMESPACE_NAME::magnitude(a - b);
}

template<is_vector2_sint TVec2Int>
sint_to_floating_point_t<vector_field_t<TVec2Int>> distance(const TVec2Int& a, const TVec2Int& b)
{
    using F = sint_to_floating_point_t<vector_field_t<TVec2Int>>;
    const TVec2Int delta = a - b;
    return TMATH_NAMESPACE_NAME::magnitude(static_cast<F>(delta.x), static_cast<F>(delta.y));
}

TMATH_NAMESPACE_END