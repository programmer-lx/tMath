#pragma once

#include <cmath>
#include <numbers>
#include <limits>

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

// ======================================== PI ========================================
template<is_floating_point F>
static constexpr F PI = std::numbers::pi_v<F>;

template<is_floating_point F>
static constexpr F InvPI = static_cast<F>(1) / PI<F>;

template<is_floating_point F>
static constexpr F TwoPI = static_cast<F>(2) * PI<F>;

template<is_floating_point F>
static constexpr F InvTwoPI = static_cast<F>(1) / TwoPI<F>;

template<is_floating_point F>
static constexpr F HalfPI = static_cast<F>(0.5) * PI<F>;



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
static constexpr F Rad2Deg = static_cast<F>(180) / PI<F>;

template<is_floating_point F>
static constexpr F Deg2Rad = PI<F> / static_cast<F>(180);


// ======================================== precision ========================================
template<is_floating_point F>
static constexpr F MinTolerance = static_cast<F>(10) * std::numeric_limits<F>::epsilon();

template<is_number N>
static constexpr N Max = std::numeric_limits<N>::max();

template<is_number N>
static constexpr N Min = std::numeric_limits<N>::min();

template<is_number N>
static constexpr N Infinity = std::numeric_limits<N>::infinity();

template<is_number N>
static constexpr N QuietNaN = std::numeric_limits<N>::quiet_NaN();

template<is_number N>
static constexpr N SignalingNaN = std::numeric_limits<N>::signaling_NaN();

template<is_number N>
static constexpr N NaN = QuietNaN<N>;

TMATH_NAMESPACE_END