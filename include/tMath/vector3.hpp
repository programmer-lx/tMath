#pragma once

#include <cmath>

#include "math_defs.hpp"

TMATH_CLANG_DIAGNOSTIC_PUSH
TMATH_IGNORE_CLANG_SUBOBJECT_BRACES_WARNING

TMATH_NAMESPACE_BEGIN

// ============================================= operators =============================================
template<is_vector3 TVec3>
constexpr bool operator==(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

template<is_vector3 TVec3>
constexpr TVec3& operator+=(TVec3& lhs, const TVec3& rhs) noexcept
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

template<is_vector3 TVec3>
constexpr TVec3& operator-=(TVec3& lhs, const TVec3& rhs) noexcept
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

template<is_vector3 TVec3, is_signed_number N>
constexpr TVec3& operator*=(TVec3& lhs, const N rhs) noexcept
{
    using Field = vector_quat_field_t<TVec3>;

    lhs.x *= static_cast<Field>(rhs);
    lhs.y *= static_cast<Field>(rhs);
    lhs.z *= static_cast<Field>(rhs);
    return lhs;
}

template<is_vector3 TVec3>
constexpr TVec3& operator/=(TVec3& lhs, const vector_quat_field_t<TVec3> rhs) noexcept
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    return lhs;
}


TMATH_NAMESPACE_END
#include "impl/vector_operators.inl"
TMATH_NAMESPACE_BEGIN



// ============================================= casts =============================================
template<is_vector3 Ret, is_vector3 In>
constexpr Ret vector_cast(const In& v) noexcept
{
    using F = vector_quat_field_t<Ret>;
    return { static_cast<F>(v.x), static_cast<F>(v.y), static_cast<F>(v.z) };
}



// ============================================= functions =============================================
template<is_vector3_floating_point TVec3>
constexpr TVec3 to_degrees(const TVec3& radians) noexcept
{
    return { to_degrees(radians.x), to_degrees(radians.y), to_degrees(radians.z) };
}

template<is_vector3_floating_point TVec3>
constexpr TVec3 to_radians(const TVec3& degrees) noexcept
{
    return { to_radians(degrees.x), to_radians(degrees.y), to_radians(degrees.z) };
}

template<is_vector3 TVec3>
TVec3 abs(const TVec3& v) noexcept
{
    return { TMATH_NAMESPACE_NAME::abs(v.x), TMATH_NAMESPACE_NAME::abs(v.y), TMATH_NAMESPACE_NAME::abs(v.z) };
}

template<is_vector3 TVec3>
constexpr TVec3 hadamard_mul(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return {
        lhs.x * rhs.x,
        lhs.y * rhs.y,
        lhs.z * rhs.z
    };
}

template<is_vector3 TVec3>
constexpr TVec3 hadamard_div(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return {
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z
    };
}

template<is_floating_point F>
constexpr F dot(
    const F x1, const F y1, const F z1,
    const F x2, const F y2, const F z2
) noexcept
{
    return  x1 * x2 +
            y1 * y2 +
            z1 * z2;
}

template<is_vector3 TVec3>
constexpr vector_quat_field_t<TVec3> dot(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return  lhs.x * rhs.x +
            lhs.y * rhs.y +
            lhs.z * rhs.z;
}

template<is_vector3 TVec3>
constexpr TVec3 cross(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

template<is_vector3_floating_point TVec3>
bool approximately(const TVec3& a, const TVec3& b, const vector_quat_field_t<TVec3> tolerance = MinTolerance<vector_quat_field_t<TVec3>>) noexcept
{
    return  approximately(a.x, b.x, tolerance) &&
            approximately(a.y, b.y, tolerance) &&
            approximately(a.z, b.z, tolerance);
}

template<is_floating_point F>
F magnitude(const F x, const F y, const F z) noexcept
{
    return std::hypot(x, y, z);
}

template<is_vector3_floating_point TVec3>
vector_quat_field_t<TVec3> magnitude(const TVec3& v) noexcept
{
    return std::hypot(v.x, v.y, v.z);
}

template<is_vector3_floating_point TVec3>
void normalize_inplace(TVec3& v) noexcept
{
    using F = vector_quat_field_t<TVec3>;
    const F inv_mag = static_cast<F>(1) / magnitude(v);
    v.x *= inv_mag;
    v.y *= inv_mag;
    v.z *= inv_mag;
}

template<is_vector3_floating_point TVec3>
void safe_normalize_inplace(TVec3& v, const TVec3& fallback) noexcept
{
    using F = vector_quat_field_t<TVec3>;
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
TVec3 normalized(const TVec3& v) noexcept
{
    TVec3 result = v;
    normalize_inplace(result);
    return result;
}

template<is_vector3_floating_point RetVec3, is_vector3_sint TVec3Int>
RetVec3 normalized(const TVec3Int& v) noexcept
{
    using F = vector_quat_field_t<RetVec3>;
    const F inv_mag = static_cast<F>(1) / magnitude(static_cast<F>(v.x), static_cast<F>(v.y), static_cast<F>(v.z));

    return {
        static_cast<F>(v.x * inv_mag),
        static_cast<F>(v.y * inv_mag),
        static_cast<F>(v.z * inv_mag)
    };
}

template<is_vector3_floating_point TVec3>
TVec3 safe_normalized(const TVec3& v, const TVec3& fallback) noexcept
{
    using F = vector_quat_field_t<TVec3>;
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
RetVec3 safe_normalized(const TVec3Int& v, const RetVec3& fallback) noexcept
{
    using F = vector_quat_field_t<RetVec3>;
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
vector_quat_field_t<TVec3> distance(const TVec3& a, const TVec3& b) noexcept
{
    return magnitude(a - b);
}

template<is_vector3_sint TVec3Int>
sint_to_floating_point_t<vector_quat_field_t<TVec3Int>> distance(const TVec3Int& a, const TVec3Int& b) noexcept
{
    using F = sint_to_floating_point_t<vector_quat_field_t<TVec3Int>>;
    const TVec3Int delta = a - b;
    return magnitude(static_cast<F>(delta.x), static_cast<F>(delta.y), static_cast<F>(delta.z));
}

template<is_vector3_floating_point TVec3, is_floating_point F>
constexpr TVec3 lerp(const TVec3& a, const TVec3& b, const F t) noexcept
{
    return {
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t),
        lerp(a.z, b.z, t)
    };
}

TMATH_NAMESPACE_END

TMATH_CLANG_DIAGNOSTIC_POP
