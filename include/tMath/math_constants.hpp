#pragma once

#include <numbers>
#include <limits>

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

template<is_floating_point F>
static constexpr F PI = std::numbers::pi_v<F>;

template<is_floating_point F>
static constexpr F Rad2Deg = static_cast<F>(180) / PI<F>;

template<is_floating_point F>
static constexpr F Deg2Rad = PI<F> / static_cast<F>(180);

template<is_floating_point F>
static constexpr F MinTolerance = 10 * std::numeric_limits<F>::epsilon();

TMATH_NAMESPACE_END