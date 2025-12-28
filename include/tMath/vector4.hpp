#pragma once

#include <cmath>

#include "math_defs.hpp"
#include "vector1.hpp"

TMATH_NAMESPACE_BEGIN

// ============================================= operators =============================================
template<is_vector4 TVec4>
constexpr bool operator==(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return  (lhs.data[0] == rhs.data[0]) &&
            (lhs.data[1] == rhs.data[1]) &&
            (lhs.data[2] == rhs.data[2]) &&
            (lhs.data[3] == rhs.data[3]);
}

template<is_vector4 TVec4>
constexpr TVec4& operator+=(TVec4& lhs, const TVec4& rhs) noexcept
{
    lhs.data[0] += rhs.data[0];
    lhs.data[1] += rhs.data[1];
    lhs.data[2] += rhs.data[2];
    lhs.data[3] += rhs.data[3];
    return lhs;
}

template<is_vector4 TVec4>
constexpr TVec4& operator-=(TVec4& lhs, const TVec4& rhs) noexcept
{
    lhs.data[0] -= rhs.data[0];
    lhs.data[1] -= rhs.data[1];
    lhs.data[2] -= rhs.data[2];
    lhs.data[3] -= rhs.data[3];
    return lhs;
}

template<is_vector4 TVec4, is_signed_number N>
constexpr TVec4& operator*=(TVec4& lhs, const N rhs) noexcept
{
    using Field = vector_quat_component_t<TVec4>;

    lhs.data[0] *= static_cast<Field>(rhs);
    lhs.data[1] *= static_cast<Field>(rhs);
    lhs.data[2] *= static_cast<Field>(rhs);
    lhs.data[3] *= static_cast<Field>(rhs);
    return lhs;
}

template<is_vector4 TVec4>
constexpr TVec4& operator/=(TVec4& lhs, const vector_quat_component_t<TVec4> rhs) noexcept
{
    lhs.data[0] /= rhs;
    lhs.data[1] /= rhs;
    lhs.data[2] /= rhs;
    lhs.data[3] /= rhs;
    return lhs;
}

TMATH_NAMESPACE_END
#include "impl/vector_operators.inl"
TMATH_NAMESPACE_BEGIN


// ============================================= casts =============================================
template<is_vector4 Ret, is_vector4 In>
constexpr Ret vector_cast(const In& v) noexcept
{
    using F = vector_quat_component_t<Ret>;
    return { static_cast<F>(v.data[0]), static_cast<F>(v.data[1]), static_cast<F>(v.data[2]), static_cast<F>(v.data[3]) };
}



// ============================================= functions =============================================
template<is_vector4_floating_point TVec4>
constexpr TVec4 to_degrees(const TVec4& radians) noexcept
{
    return { to_degrees(radians.data[0]), to_degrees(radians.data[1]), to_degrees(radians.data[2]), to_degrees(radians.data[3]) };
}

template<is_vector4_floating_point TVec4>
constexpr TVec4 to_radians(const TVec4& degrees) noexcept
{
    return { to_radians(degrees.data[0]), to_radians(degrees.data[1]), to_radians(degrees.data[2]), to_radians(degrees.data[3]) };
}

template<is_vector4 TVec4>
TVec4 abs(const TVec4& v) noexcept
{
    return {
        std::abs(v.data[0]),
        std::abs(v.data[1]),
        std::abs(v.data[2]),
        std::abs(v.data[3])
    };
}

template<is_vector4 TVec4>
TVec4 sin(const TVec4& v) noexcept
{
    return {
        std::sin(v.data[0]),
        std::sin(v.data[1]),
        std::sin(v.data[2]),
        std::sin(v.data[3])
    };
}

template<is_vector4 TVec4>
TVec4 cos(const TVec4& v) noexcept
{
    return {
        std::cos(v.data[0]),
        std::cos(v.data[1]),
        std::cos(v.data[2]),
        std::cos(v.data[3])
    };
}

template<is_vector4 TVec4>
TVec4 tan(const TVec4& v) noexcept
{
    return {
        std::tan(v.data[0]),
        std::tan(v.data[1]),
        std::tan(v.data[2]),
        std::tan(v.data[3])
    };
}

template<is_vector4 TVec4>
constexpr TVec4 hadamard_mul(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return {
        lhs.data[0] * rhs.data[0],
        lhs.data[1] * rhs.data[1],
        lhs.data[2] * rhs.data[2],
        lhs.data[3] * rhs.data[3]
    };
}

template<is_vector4 TVec4>
constexpr TVec4 hadamard_div(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return {
        lhs.data[0] / rhs.data[0],
        lhs.data[1] / rhs.data[1],
        lhs.data[2] / rhs.data[2],
        lhs.data[3] / rhs.data[3]
    };
}

template<is_signed_number N>
constexpr N dot(
    const N x1, const N y1, const N z1, const N w1,
    const N x2, const N y2, const N z2, const N w2
) noexcept
{
    return  x1 * x2 +
            y1 * y2 +
            z1 * z2 +
            w1 * w2;
}

template<is_vector4 TVec4>
constexpr vector_quat_component_t<TVec4> dot(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return  lhs.data[0] * rhs.data[0] +
            lhs.data[1] * rhs.data[1] +
            lhs.data[2] * rhs.data[2] +
            lhs.data[3] * rhs.data[3];
}

template<is_signed_number N>
constexpr N dot4(const N* lhs, const N* rhs) noexcept
{
    return  lhs[0] * rhs[0] +
            lhs[1] * rhs[1] +
            lhs[2] * rhs[2] +
            lhs[3] * rhs[3];
}

// vector3's cross, ignore w component, the return value's w is zero
template<is_vector4 TVec4>
constexpr TVec4 cross(const TVec4& lhs, const TVec4& rhs) noexcept
{
    return {
        lhs.data[1] * rhs.data[2] - lhs.data[2] * rhs.data[1],
        lhs.data[2] * rhs.data[0] - lhs.data[0] * rhs.data[2],
        lhs.data[0] * rhs.data[1] - lhs.data[1] * rhs.data[0],
        static_cast<vector_quat_component_t<TVec4>>(0)
    };
}

template<is_vector4_floating_point TVec4>
bool approximately(const TVec4& a, const TVec4& b, const vector_quat_component_t<TVec4> tolerance = MinTolerance<vector_quat_component_t<TVec4>>) noexcept
{
    return  approximately(a.data[0], b.data[0], tolerance) &&
            approximately(a.data[1], b.data[1], tolerance) &&
            approximately(a.data[2], b.data[2], tolerance) &&
            approximately(a.data[3], b.data[3], tolerance);
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
vector_quat_component_t<TVec4> magnitude(const TVec4& v) noexcept
{
    return std::sqrt(
        v.data[0] * v.data[0] +
        v.data[1] * v.data[1] +
        v.data[2] * v.data[2] +
        v.data[3] * v.data[3]
    );
}

template<is_vector4_floating_point TVec4>
void normalize_inplace(TVec4& v) noexcept
{
    using F = vector_quat_component_t<TVec4>;
    const F inv_mag = static_cast<F>(1) / magnitude(v);
    v.data[0] *= inv_mag;
    v.data[1] *= inv_mag;
    v.data[2] *= inv_mag;
    v.data[3] *= inv_mag;
}

template<is_vector4_floating_point TVec4>
void safe_normalize_inplace(TVec4& v, const TVec4& fallback) noexcept
{
    using F = vector_quat_component_t<TVec4>;
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
    v.data[3] *= inv_mag;
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
    using F = vector_quat_component_t<RetVec3>;
    const F inv_mag = static_cast<F>(1) / magnitude(static_cast<F>(v.data[0]), static_cast<F>(v.data[1]), static_cast<F>(v.data[2]), static_cast<F>(v.data[3]));

    return {
        static_cast<F>(v.data[0] * inv_mag),
        static_cast<F>(v.data[1] * inv_mag),
        static_cast<F>(v.data[2] * inv_mag),
        static_cast<F>(v.data[3] * inv_mag)
    };
}

template<is_vector4_floating_point TVec4>
TVec4 safe_normalized(const TVec4& v, const TVec4& fallback) noexcept
{
    using F = vector_quat_component_t<TVec4>;
    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.data[0] * inv_mag),
        static_cast<F>(v.data[1] * inv_mag),
        static_cast<F>(v.data[2] * inv_mag),
        static_cast<F>(v.data[3] * inv_mag)
    };
}

template<is_vector4_floating_point RetVec3, is_vector4_sint TVec4Int>
RetVec3 safe_normalized(const TVec4Int& v, const RetVec3& fallback) noexcept
{
    using F = vector_quat_component_t<RetVec3>;
    const F mag = magnitude(static_cast<F>(v.data[0]), static_cast<F>(v.data[1]), static_cast<F>(v.data[2]), static_cast<F>(v.data[3]));
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return {
        static_cast<F>(v.data[0] * inv_mag),
        static_cast<F>(v.data[1] * inv_mag),
        static_cast<F>(v.data[2] * inv_mag),
        static_cast<F>(v.data[3] * inv_mag)
    };
}

template<is_vector4_floating_point TVec4>
vector_quat_component_t<TVec4> distance(const TVec4& a, const TVec4& b) noexcept
{
    return magnitude(a - b);
}

template<is_vector4_sint TVec4Int>
sint_to_floating_point_t<vector_quat_component_t<TVec4Int>> distance(const TVec4Int& a, const TVec4Int& b) noexcept
{
    using F = sint_to_floating_point_t<vector_quat_component_t<TVec4Int>>;
    const TVec4Int delta = a - b;
    return magnitude(static_cast<F>(delta.data[0]), static_cast<F>(delta.data[1]), static_cast<F>(delta.data[2]), static_cast<F>(delta.data[3]));
}

template<is_vector4_floating_point TVec4, is_floating_point F>
constexpr TVec4 lerp(const TVec4& a, const TVec4& b, const F t) noexcept
{
    return {
        lerp(a.data[0], b.data[0], t),
        lerp(a.data[1], b.data[1], t),
        lerp(a.data[2], b.data[2], t),
        lerp(a.data[3], b.data[3], t)
    };
}


TMATH_NAMESPACE_END
