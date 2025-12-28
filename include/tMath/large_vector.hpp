#pragma once

#include <utility>

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

namespace detail
{
    template<is_signed_number N, size_t... I>
    constexpr N dot_n_impl(const N* lhs, const N* rhs, std::index_sequence<I...>) noexcept
    {
        return ( (lhs[I] * rhs[I]) + ... );
    }
}

template<int Size, is_signed_number N>
constexpr N dot_n(const N* lhs, const N* rhs) noexcept
{
    return detail::dot_n_impl(lhs, rhs, std::make_index_sequence<Size>{});
}

TMATH_NAMESPACE_END
