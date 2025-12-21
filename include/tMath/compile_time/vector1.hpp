#pragma once

#include "defs.hpp"
#include "../vector1.hpp"

TMATH_COMPILE_TIME_NAMESPACE_BEGIN

template<is_floating_point F>
constexpr F fmod(const F x, const F y) noexcept
{
    if (y == static_cast<F>(0))
    {
        return QuietNaN<F>;
    }

    const long double q = x / y;

    // trunc(q)
    const int64_t iq =
        q >= F(0)
            ? static_cast<int64_t>(q)
            : -static_cast<int64_t>(-q);

    return x - static_cast<F>(iq) * y;
}

template<is_signed_number N>
constexpr N abs(const N x) noexcept
{
    return x < 0 ? -x : x;
}

TMATH_COMPILE_TIME_NAMESPACE_END
