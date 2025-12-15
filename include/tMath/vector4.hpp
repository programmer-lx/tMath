#pragma once

#include <cmath>

#include "math_defs.hpp"
#include "vector1.hpp"

TMATH_NAMESPACE_BEGIN

// ============================================= operators =============================================
template<is_vector4 TVec4>
bool operator==(const TVec4& lhs, const TVec4& rhs)
{
    return  (lhs.x == rhs.x) &&
            (lhs.y == rhs.y) &&
            (lhs.z == rhs.z) &&
            (lhs.w == rhs.w);
}

template<is_vector4 TVec4>
bool operator!=(const TVec4& lhs, const TVec4& rhs)
{
    return !(lhs == rhs);
}

template<is_vector4 TVec4>
TVec4& operator+=(TVec4& lhs, const TVec4& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}

template<is_vector4 TVec4>
TVec4& operator-=(TVec4& lhs, const TVec4& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;
    return lhs;
}

template<is_vector4 TVec4>
TVec4& operator*=(TVec4& lhs, const vector_field_t<TVec4> rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    lhs.w *= rhs;
    return lhs;
}

template<is_vector4 TVec4>
TVec4& operator/=(TVec4& lhs, const vector_field_t<TVec4> rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    lhs.w /= rhs;
    return lhs;
}

template<is_vector4 TVec4>
void safe_divide_inplace(TVec4& v, const vector_field_t<TVec4> divisor, const TVec4& fallback)
{
    if (is_invalid_divisor(divisor))
    {
        v = fallback;
        return;
    }

    v /= divisor;
}

template<is_vector4 TVec4>
TVec4 operator+(const TVec4& lhs, const TVec4& rhs)
{
    TVec4 v = lhs;
    v += rhs;
    return v;
}

template<is_vector4 TVec4>
TVec4 operator-(const TVec4& lhs, const TVec4& rhs)
{
    TVec4 v = lhs;
    v -= rhs;
    return v;
}

template<is_vector4 TVec4>
TVec4 operator*(const TVec4& lhs, const vector_field_t<TVec4> rhs)
{
    TVec4 v = lhs;
    v *= rhs;
    return v;
}

template<is_vector4 TVec4>
TVec4 operator/(const TVec4& lhs, const vector_field_t<TVec4> rhs)
{
    TVec4 v = lhs;
    v /= rhs;
    return v;
}

template<is_vector4 TVec4>
TVec4 safe_divide(const TVec4& v, const vector_field_t<TVec4> divisor, const TVec4& fallback)
{
    TVec4 result = v;
    safe_divide_inplace(result, divisor, fallback);
    return result;
}

template<is_vector4 TVec4>
vector_field_t<TVec4> dot(const TVec4& lhs, const TVec4& rhs)
{
    return  lhs.x * rhs.x +
            lhs.y * rhs.y +
            lhs.z * rhs.z +
            lhs.w * rhs.w;
}

// vector3's cross, ignore w component, the return value's w is zero
template<is_vector4 TVec4>
TVec4 cross(const TVec4& lhs, const TVec4& rhs)
{
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
        static_cast<vector_field_t<TVec4>>(0)
    };
}



// ============================================= casts =============================================
template<is_vector4 Ret, is_vector4 In>
Ret precision_cast(const In& v)
{
    using F = vector_field_t<Ret>;
    return { static_cast<F>(v.x), static_cast<F>(v.y), static_cast<F>(v.z), static_cast<F>(v.w) };
}




template<is_vector4_floating_point TVec4>
bool approximately(const TVec4& a, const TVec4& b, const vector_field_t<TVec4> tolerance = MinTolerance<vector_field_t<TVec4>>)
{
    return  approximately(a.x, b.x, tolerance) &&
            approximately(a.y, b.y, tolerance) &&
            approximately(a.z, b.z, tolerance) &&
            approximately(a.w, b.w, tolerance);
}


TMATH_NAMESPACE_END