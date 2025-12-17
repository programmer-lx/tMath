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
F sin(const F f)
{
    return std::sin(f);
}

template<is_floating_point F>
F asin(const F f)
{
    return std::asin(f);
}

template<is_floating_point F>
F cos(const F f)
{
    return std::cos(f);
}

template<is_floating_point F>
F acos(const F f)
{
    return std::acos(f);
}

template<is_floating_point F>
F tan(const F f)
{
    return std::tan(f);
}

template<is_floating_point F>
F atan(const F f)
{
    return std::atan(f);
}

template<is_floating_point F>
F atan2(const F y, const F x)
{
    return std::atan2(y, x);
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

template<is_floating_point F1, is_floating_point F2>
bool approximately(const F1 a, const F2 b, const min_floating_point_t<F1, F2> tolerance = MinTolerance<min_floating_point_t<F1, F2>>)
{
    using F = max_floating_point_t<F1, F2>;
    return TMATH_NAMESPACE_NAME::abs(static_cast<F>(a) - static_cast<F>(b)) < tolerance;
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
