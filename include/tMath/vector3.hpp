#pragma once

#include <cmath>

#include "math_defs.hpp"
#include "vector1.hpp"

TMATH_NAMESPACE_BEGIN

// ============================================= operators =============================================
template<is_vector3 TVec3>
constexpr bool operator==(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return (lhs.data[0] == rhs.data[0]) && (lhs.data[1] == rhs.data[1]) && (lhs.data[2] == rhs.data[2]);
}

template<is_vector3 TVec3>
constexpr TVec3& operator+=(TVec3& lhs, const TVec3& rhs) noexcept
{
    lhs.data[0] += rhs.data[0];
    lhs.data[1] += rhs.data[1];
    lhs.data[2] += rhs.data[2];
    return lhs;
}

template<is_vector3 TVec3>
constexpr TVec3& operator-=(TVec3& lhs, const TVec3& rhs) noexcept
{
    lhs.data[0] -= rhs.data[0];
    lhs.data[1] -= rhs.data[1];
    lhs.data[2] -= rhs.data[2];
    return lhs;
}

template<is_vector3 TVec3, is_signed_number N>
constexpr TVec3& operator*=(TVec3& lhs, const N rhs) noexcept
{
    using Comp = vector_component_t<TVec3>;

    lhs.data[0] *= static_cast<Comp>(rhs);
    lhs.data[1] *= static_cast<Comp>(rhs);
    lhs.data[2] *= static_cast<Comp>(rhs);
    return lhs;
}

template<is_vector3 TVec3>
constexpr TVec3& operator/=(TVec3& lhs, const vector_component_t<TVec3> rhs) noexcept
{
    lhs.data[0] /= rhs;
    lhs.data[1] /= rhs;
    lhs.data[2] /= rhs;
    return lhs;
}


TMATH_NAMESPACE_END
#include "impl/vector_operators.inl"
TMATH_NAMESPACE_BEGIN



// ============================================= casts =============================================
template<is_vector3 Ret, is_vector3 In>
constexpr Ret vector_cast(const In& v) noexcept
{
    using F = vector_component_t<Ret>;
    return { static_cast<F>(v.data[0]), static_cast<F>(v.data[1]), static_cast<F>(v.data[2]) };
}



// ============================================= functions =============================================
template<is_vector3_floating_point TVec3>
constexpr TVec3 to_degrees(const TVec3& radians) noexcept
{
    return { to_degrees(radians.data[0]), to_degrees(radians.data[1]), to_degrees(radians.data[2]) };
}

template<is_vector3_floating_point TVec3>
constexpr TVec3 to_radians(const TVec3& degrees) noexcept
{
    return { to_radians(degrees.data[0]), to_radians(degrees.data[1]), to_radians(degrees.data[2]) };
}

template<is_vector3 TVec3>
TVec3 abs(const TVec3& v) noexcept
{
    return { std::abs(v.data[0]), std::abs(v.data[1]), std::abs(v.data[2]) };
}

template<is_vector3 TVec3>
constexpr TVec3 hadamard_mul(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return {
        lhs.data[0] * rhs.data[0],
        lhs.data[1] * rhs.data[1],
        lhs.data[2] * rhs.data[2]
    };
}

template<is_vector3 TVec3>
constexpr TVec3 hadamard_div(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return {
        lhs.data[0] / rhs.data[0],
        lhs.data[1] / rhs.data[1],
        lhs.data[2] / rhs.data[2]
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
constexpr vector_component_t<TVec3> dot(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return  lhs.data[0] * rhs.data[0] +
            lhs.data[1] * rhs.data[1] +
            lhs.data[2] * rhs.data[2];
}

template<is_vector3 TVec3>
constexpr TVec3 cross(const TVec3& lhs, const TVec3& rhs) noexcept
{
    return {
        lhs.data[1] * rhs.data[2] - lhs.data[2] * rhs.data[1],
        lhs.data[2] * rhs.data[0] - lhs.data[0] * rhs.data[2],
        lhs.data[0] * rhs.data[1] - lhs.data[1] * rhs.data[0]
    };
}

template<is_vector3_floating_point TVec3>
bool approximately(const TVec3& a, const TVec3& b, const vector_component_t<TVec3> tolerance = MinTolerance<vector_component_t<TVec3>>) noexcept
{
    return  approximately(a.data[0], b.data[0], tolerance) &&
            approximately(a.data[1], b.data[1], tolerance) &&
            approximately(a.data[2], b.data[2], tolerance);
}

template<is_floating_point F>
F magnitude(const F x, const F y, const F z) noexcept
{
    return std::hypot(x, y, z);
}

template<is_vector3_floating_point TVec3>
vector_component_t<TVec3> magnitude(const TVec3& v) noexcept
{
    return std::hypot(v.data[0], v.data[1], v.data[2]);
}

template<is_vector3_floating_point TVec3>
void normalize_inplace(TVec3& v) noexcept
{
    using F = vector_component_t<TVec3>;
    const F inv_mag = static_cast<F>(1) / magnitude(v);
    v.data[0] *= inv_mag;
    v.data[1] *= inv_mag;
    v.data[2] *= inv_mag;
}

template<is_vector3_floating_point TVec3>
void safe_normalize_inplace(TVec3& v, const TVec3& fallback) noexcept
{
    using F = vector_component_t<TVec3>;
    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        v = fallback;
        return;
    }

    const F inv_mag = static_cast<F>(1) / mag;
    v.data[0] *= inv_mag;
    v.data[1] *= inv_mag;
    v.data[2] *= inv_mag;
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
    using F = vector_component_t<RetVec3>;
    const F inv_mag = static_cast<F>(1) / magnitude(static_cast<F>(v.data[0]), static_cast<F>(v.data[1]), static_cast<F>(v.data[2]));

    return {
        static_cast<F>(v.data[0] * inv_mag),
        static_cast<F>(v.data[1] * inv_mag),
        static_cast<F>(v.data[2] * inv_mag)
    };
}

template<is_vector3_floating_point TVec3>
TVec3 safe_normalized(const TVec3& v, const TVec3& fallback) noexcept
{
    using F = vector_component_t<TVec3>;
    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.data[0] * inv_mag),
        static_cast<F>(v.data[1] * inv_mag),
        static_cast<F>(v.data[2] * inv_mag)
    };
}

template<is_vector3_floating_point RetVec3, is_vector3_sint TVec3Int>
RetVec3 safe_normalized(const TVec3Int& v, const RetVec3& fallback) noexcept
{
    using F = vector_component_t<RetVec3>;
    const F mag = magnitude(static_cast<F>(v.data[0]), static_cast<F>(v.data[1]), static_cast<F>(v.data[2]));
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.data[0] * inv_mag),
        static_cast<F>(v.data[1] * inv_mag),
        static_cast<F>(v.data[2] * inv_mag)
    };
}

template<is_vector3_floating_point TVec3>
vector_component_t<TVec3> distance(const TVec3& a, const TVec3& b) noexcept
{
    return magnitude(a - b);
}

template<is_vector3_sint TVec3Int>
sint_to_floating_point_t<vector_component_t<TVec3Int>> distance(const TVec3Int& a, const TVec3Int& b) noexcept
{
    using F = sint_to_floating_point_t<vector_component_t<TVec3Int>>;
    const TVec3Int delta = a - b;
    return magnitude(static_cast<F>(delta.data[0]), static_cast<F>(delta.data[1]), static_cast<F>(delta.data[2]));
}

template<is_vector3_floating_point TVec3, is_floating_point F>
constexpr TVec3 lerp(const TVec3& a, const TVec3& b, const F t) noexcept
{
    return {
        lerp(a.data[0], b.data[0], t),
        lerp(a.data[1], b.data[1], t),
        lerp(a.data[2], b.data[2], t)
    };
}


// 预设类型
struct Vector3f
{
    TMATH_FULL_VECTOR3(Vector3f, float)
};

TMATH_NAMESPACE_END
