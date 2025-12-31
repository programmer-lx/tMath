#pragma once

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <numbers>
#include <limits>
#include <cstring>

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

// ======================================== constants ========================================

// ======================================== PI ========================================
template<is_floating_point F>
static constexpr F PI = std::numbers::pi_v<F>;

template<is_floating_point F>
static constexpr F InvPI = std::numbers::inv_pi_v<F>;

template<is_floating_point F>
static constexpr F TwoPI = static_cast<F>(2) * PI<F>;

template<is_floating_point F>
static constexpr F InvTwoPI = static_cast<F>(1) / TwoPI<F>;

template<is_floating_point F>
static constexpr F HalfPI = PI<F> / static_cast<F>(2);

template<is_floating_point F>
static constexpr F QuarterPI = PI<F> / static_cast<F>(4);



// ======================================== e ========================================
template<is_floating_point F>
static constexpr F e = std::numbers::e_v<F>;



// ======================================== log ========================================
template<is_floating_point F>
static constexpr F ln2 = std::numbers::ln2_v<F>;

template<is_floating_point F>
static constexpr F ln10 = std::numbers::ln10_v<F>;

template<is_floating_point F>
static constexpr F log2e = std::numbers::log2e_v<F>;

template<is_floating_point F>
static constexpr F log10e = std::numbers::log10e_v<F>;



// ======================================== angle ========================================
template<is_floating_point F>
static constexpr F Rad2Deg = static_cast<F>(57.295779513082320876798154814105L);

template<is_floating_point F>
static constexpr F Deg2Rad = static_cast<F>(0.01745329251994329576923690768489L);


// ======================================== precision ========================================
template<is_signed_number N>
static constexpr N Epsilon = std::numeric_limits<N>::epsilon();

template<is_signed_number N>
static constexpr N Max = std::numeric_limits<N>::max();

template<is_signed_number N>
static constexpr N Min = std::numeric_limits<N>::min();

template<is_signed_number N>
static constexpr N Infinity = std::numeric_limits<N>::infinity();

template<is_signed_number N>
static constexpr N QuietNaN = std::numeric_limits<N>::quiet_NaN();

template<is_signed_number N>
static constexpr N SignalingNaN = std::numeric_limits<N>::signaling_NaN();

template<is_signed_number N>
static constexpr N NaN = QuietNaN<N>;


// ======================================== floating point functions ======================================

using std::abs;
using std::exp;
using std::sin;
using std::asin;
using std::cos;
using std::acos;
using std::tan;
using std::atan;
using std::atan2;
using std::sqrt;

using std::min;
using std::max;


// ======================================== functions ========================================

template<is_floating_point F>
F one_block() noexcept
{
    F f;
    // memset 的第二个参数 value, 只用其低8位
    std::memset(&f, 0xff, sizeof(F));
    return f;
}

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

/**
 * clamp n to [min, max]
 */
template<is_signed_number N>
constexpr N clamp(const N n, const N min, const N max) noexcept
{
    return (n < min) ? min : (n > max) ? max : n;
}

template<is_floating_point F, is_floating_point F2>
constexpr F lerp(const F a, const F b, const F2 t) noexcept
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

template<is_floating_point F1, is_floating_point F2>
bool approximately(const F1 a, const F2 b, const min_floating_point_t<F1, F2> tolerance = Epsilon<min_floating_point_t<F1, F2>>) noexcept
{
    using max_float_t = max_floating_point_t<F1, F2>;

    return TMATH_NAMESPACE_NAME::abs(static_cast<max_float_t>(a) - static_cast<max_float_t>(b)) <= static_cast<max_float_t>(tolerance);
}

template<is_floating_point N>
bool is_infinity(N n) noexcept
{
    return std::isinf(n);
}

template<is_floating_point N>
bool is_finite(N n) noexcept
{
    return std::isfinite(n);
}

template<is_floating_point N>
bool is_nan(N n) noexcept
{
    return std::isnan(n);
}

// 无效除数
template<is_floating_point F>
bool is_invalid_divisor(F f) noexcept
{
    return !is_finite(f) || (abs(f) <= Epsilon<F>);
}

template<is_int I>
bool is_invalid_divisor(I i) noexcept
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
