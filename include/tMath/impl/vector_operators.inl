#pragma once

#ifndef TMATH_VECTOR_OPERATORS_HEADER__
#define TMATH_VECTOR_OPERATORS_HEADER__

#include "../math_defs.hpp"

TMATH_NAMESPACE_BEGIN

template<is_vector_n TVec>
constexpr bool operator!=(const TVec& lhs, const TVec& rhs) noexcept
{
    return !(lhs == rhs);
}

template<is_vector_n TVec>
void safe_divide_inplace(TVec& v, const vector_field_t<TVec> divisor, const TVec& fallback) noexcept
{
    if (is_invalid_divisor(divisor))
    {
        v = fallback;
        return;
    }

    v /= divisor;
}

template<is_vector_n TVec>
constexpr TVec operator+(const TVec& lhs, const TVec& rhs) noexcept
{
    TVec v = lhs;
    v += rhs;
    return v;
}

template<is_vector_n TVec>
constexpr TVec operator-(const TVec& lhs, const TVec& rhs) noexcept
{
    TVec v = lhs;
    v -= rhs;
    return v;
}

template<is_vector_n TVec>
constexpr TVec operator*(const TVec& lhs, const vector_field_t<TVec> rhs) noexcept
{
    TVec v = lhs;
    v *= rhs;
    return v;
}

template<is_vector_n TVec>
constexpr TVec operator/(const TVec& lhs, const vector_field_t<TVec> rhs) noexcept
{
    TVec v = lhs;
    v /= rhs;
    return v;
}

template<is_vector_n TVec>
TVec safe_divide(const TVec& v, const vector_field_t<TVec> divisor, const TVec& fallback) noexcept
{
    TVec result = v;
    safe_divide_inplace(result, divisor, fallback);
    return result;
}

TMATH_NAMESPACE_END

#endif // TMATH_VECTOR_OPERATORS_HEADER__