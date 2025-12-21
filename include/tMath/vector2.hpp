#pragma once

#include <cmath>

#include "math_defs.hpp"
#include "vector1.hpp"

TMATH_NAMESPACE_BEGIN

// ============================================= operators =============================================
template<is_vector2 TVec2>
constexpr bool operator==(const TVec2& lhs, const TVec2& rhs) noexcept
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

template<is_vector2 TVec2>
constexpr TVec2& operator+=(TVec2& lhs, const TVec2& rhs) noexcept
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

template<is_vector2 TVec2>
constexpr TVec2& operator-=(TVec2& lhs, const TVec2& rhs) noexcept
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

template<is_vector2 TVec2, is_signed_number N>
constexpr TVec2& operator*=(TVec2& lhs, const N rhs) noexcept
{
    using Field = vector_quat_field_t<TVec2>;

    lhs.x *= static_cast<Field>(rhs);
    lhs.y *= static_cast<Field>(rhs);
    return lhs;
}

template<is_vector2 TVec2>
constexpr TVec2& operator/=(TVec2& lhs, const vector_quat_field_t<TVec2> rhs) noexcept
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    return lhs;
}


TMATH_NAMESPACE_END
#include "impl/vector_operators.inl"
TMATH_NAMESPACE_BEGIN




// ============================================= casts =============================================
template<is_vector2 Ret, is_vector2 In>
constexpr Ret vector_cast(const In& v) noexcept
{
    using F = vector_quat_field_t<Ret>;
    return { static_cast<F>(v.x), static_cast<F>(v.y) };
}



// ============================================= functions =============================================
template<is_vector2_floating_point TVec2>
constexpr TVec2 to_degrees(const TVec2& radians) noexcept
{
    return { to_degrees(radians.x), to_degrees(radians.y) };
}

template<is_vector2_floating_point TVec2>
constexpr TVec2 to_radians(const TVec2& degrees) noexcept
{
    return { to_radians(degrees.x), to_radians(degrees.y) };
}

template<is_vector2 TVec2>
TVec2 abs(const TVec2& v) noexcept
{
    return { TMATH_NAMESPACE_NAME::abs(v.x), TMATH_NAMESPACE_NAME::abs(v.y) };
}

template<is_vector2 TVec2>
constexpr TVec2 hadamard_mul(const TVec2& lhs, const TVec2& rhs) noexcept
{
    return {
        lhs.x * rhs.x,
        lhs.y * rhs.y
    };
}

template<is_vector2 TVec2>
constexpr TVec2 hadamard_div(const TVec2& lhs, const TVec2& rhs) noexcept
{
    return {
        lhs.x / rhs.x,
        lhs.y / rhs.y
    };
}

template<is_floating_point F>
constexpr F dot(
    const F x1, const F y1,
    const F x2, const F y2
) noexcept
{
    return x1 * x2 + y1 * y2;
}

template<is_vector2 TVec2>
constexpr vector_quat_field_t<TVec2> dot(const TVec2& lhs, const TVec2& rhs) noexcept
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<is_floating_point F>
constexpr F cross(
    const F x1, const F y1,
    const F x2, const F y2
) noexcept
{
    return x1 * y2 - x2 * y1;
}

template<is_vector2 TVec2>
constexpr vector_quat_field_t<TVec2> cross(const TVec2& lhs, const TVec2& rhs) noexcept
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

template<is_vector2_floating_point TVec2>
bool approximately(const TVec2& a, const TVec2& b, const vector_quat_field_t<TVec2> tolerance = MinTolerance<vector_quat_field_t<TVec2>>) noexcept
{
    return approximately(a.x, b.x, tolerance) && approximately(a.y, b.y, tolerance);
}

template<is_floating_point F>
F magnitude(const F x, const F y) noexcept
{
    return std::hypot(x, y);
}

template<is_vector2_floating_point TVec2>
vector_quat_field_t<TVec2> magnitude(const TVec2& v) noexcept
{
    return std::hypot(v.x, v.y);
}

template<is_vector2_floating_point TVec2>
void normalize_inplace(TVec2& v) noexcept
{
    using F = vector_quat_field_t<TVec2>;
    const F inv_mag = static_cast<F>(1) / magnitude(v);
    v.x *= inv_mag;
    v.y *= inv_mag;
}

template<is_vector2_floating_point TVec2>
void safe_normalize_inplace(TVec2& v, const TVec2& fallback) noexcept
{
    using F = vector_quat_field_t<TVec2>;
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
TVec2 normalized(const TVec2& v) noexcept
{
    TVec2 result = v;
    normalize_inplace(result);
    return result;
}

template<is_vector2_floating_point RetVec2, is_vector2_sint TVec2Int>
RetVec2 normalized(const TVec2Int& v) noexcept
{
    using F = vector_quat_field_t<RetVec2>;
    const F inv_mag = static_cast<F>(1) / magnitude(static_cast<F>(v.x), static_cast<F>(v.y));

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag)
    };
}

template<is_vector2_floating_point TVec2>
TVec2 safe_normalized(const TVec2& v, const TVec2& fallback) noexcept
{
    using F = vector_quat_field_t<TVec2>;
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
RetVec2 safe_normalized(const TVec2Int& v, const RetVec2& fallback) noexcept
{
    using F = vector_quat_field_t<RetVec2>;
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
vector_quat_field_t<TVec2> distance(const TVec2& a, const TVec2& b) noexcept
{
    return magnitude(a - b);
}

template<is_vector2_sint TVec2Int>
sint_to_floating_point_t<vector_quat_field_t<TVec2Int>> distance(const TVec2Int& a, const TVec2Int& b) noexcept
{
    using F = sint_to_floating_point_t<vector_quat_field_t<TVec2Int>>;
    const TVec2Int delta = a - b;
    return magnitude(static_cast<F>(delta.x), static_cast<F>(delta.y));
}

template<is_vector2_floating_point TVec2, is_floating_point F>
constexpr TVec2 lerp(const TVec2& a, const TVec2& b, const F t) noexcept
{
    return {
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t)
    };
}

TMATH_NAMESPACE_END