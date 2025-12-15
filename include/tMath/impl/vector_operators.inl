#pragma once

#ifndef TMATH_VECTOR_OPERATORS_HEADER__
#define TMATH_VECTOR_OPERATORS_HEADER__

#include "../math_defs.hpp"

TMATH_NAMESPACE_BEGIN

template<is_vector_n TVec>
bool operator!=(const TVec& lhs, const TVec& rhs)
{
    return !(lhs == rhs);
}

template<is_vector_n TVec>
void safe_divide_inplace(TVec& v, const vector_field_t<TVec> divisor, const TVec& fallback)
{
    if (is_invalid_divisor(divisor))
    {
        v = fallback;
        return;
    }

    v /= divisor;
}

template<is_vector_n TVec>
TVec operator+(const TVec& lhs, const TVec& rhs)
{
    TVec v = lhs;
    v += rhs;
    return v;
}

template<is_vector_n TVec>
TVec operator-(const TVec& lhs, const TVec& rhs)
{
    TVec v = lhs;
    v -= rhs;
    return v;
}

template<is_vector_n TVec>
TVec operator*(const TVec& lhs, const vector_field_t<TVec> rhs)
{
    TVec v = lhs;
    v *= rhs;
    return v;
}

template<is_vector_n TVec>
TVec operator/(const TVec& lhs, const vector_field_t<TVec> rhs)
{
    TVec v = lhs;
    v /= rhs;
    return v;
}

template<is_vector_n TVec>
TVec safe_divide(const TVec& v, const vector_field_t<TVec> divisor, const TVec& fallback)
{
    TVec result = v;
    safe_divide_inplace(result, divisor, fallback);
    return result;
}

TMATH_NAMESPACE_END

#endif // TMATH_VECTOR_OPERATORS_HEADER__