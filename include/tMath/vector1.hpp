#pragma once

#include <algorithm>
#include <cmath>

#include "math_constants.hpp"
#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

template<is_floating_point F>
constexpr F to_degrees(const F radians) noexcept
{
    return radians * Rad2Deg<F>;
}

template<is_floating_point F>
constexpr F to_radians(const F degrees) noexcept
{
    return degrees * Deg2Rad<F>;
}

/**
 * x阶乘
 * @return x * (x-1) * (x-2) ... * 2 * 1
 */
template<is_int I>
constexpr I factorial(const I x) noexcept
{
    I result = static_cast<I>(1);
    for (I i = 1; i <= x; ++i)
    {
        result *= i;
    }
    return result;
}

template<is_number N>
constexpr N min(const N a, const N b) noexcept
{
    return a < b ? a : b;
}

template<is_number N>
constexpr N max(const N a, const N b) noexcept
{
    return a > b ? a : b;
}

template<is_signed_number N>
N abs(const N n) noexcept
{
    return std::abs(n);
}

template<is_floating_point F>
F exp(const F x) noexcept
{
    return std::exp(x);
}

template<is_floating_point F>
F sin(const F f) noexcept
{
    return std::sin(f);
}

template<is_floating_point F>
F asin(const F f) noexcept
{
    return std::asin(f);
}

template<is_floating_point F>
F cos(const F f) noexcept
{
    return std::cos(f);
}

template<is_floating_point F>
F acos(const F f) noexcept
{
    return std::acos(f);
}

template<is_floating_point F>
F tan(const F f) noexcept
{
    return std::tan(f);
}

template<is_floating_point F>
F atan(const F f) noexcept
{
    return std::atan(f);
}

template<is_floating_point F>
F atan2(const F y, const F x) noexcept
{
    return std::atan2(y, x);
}

template<is_floating_point F>
F sqrt(const F f) noexcept
{
    return std::sqrt(f);
}

/**
 * clamp n to [min, max]
 * if n is NaN, return NaN
 */
template<is_signed_number N>
constexpr N clamp(const N n, const N min, const N max) noexcept
{
    return (n < min) ? min : (n > max) ? max : n;
}

template<is_floating_point F, is_floating_point T>
constexpr F lerp(const F a, const F b, const T t) noexcept
{
    return a + (b - a) * static_cast<F>(t);
}

/**
 * clamp t to [0, 1] at first
 */
template<is_floating_point F, is_floating_point T>
constexpr F lerp_saturated(const F a, const F b, const T t) noexcept
{
    return lerp(a, b, clamp(t, static_cast<T>(0), static_cast<T>(1)));
}

// 类型不同的浮点的比较
template<is_floating_point F1, is_floating_point F2>
    requires (!std::is_same_v<F1, F2>)
bool approximately(const F1 a, const F2 b, const min_floating_point_t<F1, F2> tolerance = MinTolerance<min_floating_point_t<F1, F2>>) noexcept
{
    using max_float_t = max_floating_point_t<F1, F2>;
    return std::abs(static_cast<max_float_t>(a) - static_cast<max_float_t>(b)) < static_cast<max_float_t>(tolerance);
}

template<is_floating_point F>
bool approximately(const F a, const F b, const F tolerance = MinTolerance<F>) noexcept
{
    return std::abs(a - b) < tolerance;
}

template<is_floating_point N>
static bool is_infinity(N n) noexcept
{
    return std::isinf(n);
}

template<is_floating_point N>
static bool is_finite(N n) noexcept
{
    return std::isfinite(n);
}

template<is_floating_point N>
static bool is_nan(N n) noexcept
{
    return std::isnan(n);
}

// 无效除数
template<is_floating_point F>
static bool is_invalid_divisor(F f) noexcept
{
    return !is_finite(f) || (abs(f) <= MinTolerance<F>);
}

template<is_signed_int I>
static bool is_invalid_divisor(I i) noexcept
{
    return i == static_cast<I>(0);
}

template<is_signed_number N>
void safe_divide_inplace(N& val, const N divisor, const N fallback) noexcept
{
    if (is_invalid_divisor(divisor))
    {
        val = fallback;
        return;
    }

    val /= divisor;
}

template<is_signed_number N>
N safe_divide(const N val, const N divisor, const N fallback) noexcept
{
    if (is_invalid_divisor(divisor))
    {
        return fallback;
    }

    return val / divisor;
}

TMATH_NAMESPACE_END
