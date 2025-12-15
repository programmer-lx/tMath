#pragma once

#include <algorithm>
#include <cmath>

#include "math_constants.hpp"
#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

template<is_floating_point F>
F to_degrees(const F radians)
{
    return radians * Rad2Deg<F>;
}

template<is_floating_point F>
F to_radians(const F degrees)
{
    return degrees * Deg2Rad<F>;
}

template<is_number N>
N abs(const N n)
{
    return std::abs(n);
}

template<is_floating_point F>
F sqrt(const F f)
{
    return std::sqrt(f);
}

template<is_number N>
N clamp(const N n, const N min, const N max)
{
    return std::clamp(n, min, max);
}

template<is_floating_point F, is_floating_point T>
F lerp(const F a, const F b, const T t)
{
    return std::lerp(a, b, t);
}

template<is_floating_point F>
bool approximately(const F a, const F b, const F tolerance = MinTolerance<F>)
{
    return TMATH_NAMESPACE_NAME::abs(a - b) < tolerance;
}

template<is_floating_point N>
static constexpr bool is_infinity(N n)
{
    return std::isinf(n);
}

template<is_floating_point N>
static constexpr bool is_finite(N n)
{
    return std::isfinite(n);
}

template<is_floating_point N>
static constexpr bool is_nan(N n)
{
    return std::isnan(n);
}

// 无效除数
template<is_floating_point F>
static constexpr bool is_invalid_divisor(F f)
{
    return !is_finite(f) || (abs(f) <= MinTolerance<F>);
}

template<is_signed_int I>
static constexpr bool is_invalid_divisor(I i)
{
    return i == static_cast<I>(0);
}

template<is_number N>
void safe_divide_inplace(N& val, const N divisor, const N fallback)
{
    if (is_invalid_divisor(divisor))
    {
        val = fallback;
        return;
    }

    val /= divisor;
}

template<is_number N>
N safe_divide(const N val, const N divisor, const N fallback)
{
    if (is_invalid_divisor(divisor))
    {
        return fallback;
    }

    return val / divisor;
}

TMATH_NAMESPACE_END
