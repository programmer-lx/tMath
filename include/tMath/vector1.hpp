#pragma once

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

template<is_floating_point F>
bool approximately(const F a, const F b, const F tolerance = MinTolerance<F>)
{
    return TMATH_NAMESPACE_NAME::abs(a - b) < tolerance;
}

TMATH_NAMESPACE_END
