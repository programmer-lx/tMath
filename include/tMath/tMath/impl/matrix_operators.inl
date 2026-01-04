#pragma once

#include <utility>

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

namespace detail
{
    template<is_matrix_any_major Mat, size_t... I>
    constexpr bool matrix_equal_impl(const Mat& lhs, const Mat& rhs, std::index_sequence<I...>) noexcept
    {
        return (
            (lhs.data[I] == rhs.data[I]) && ...
        );
    }
}
template<is_matrix_any_major Mat>
constexpr bool operator==(const Mat& lhs, const Mat& rhs) noexcept
{
    constexpr int len = matrix_traits<Mat>::vector_count;

    return detail::matrix_equal_impl(lhs, rhs, std::make_index_sequence<len>{});
}

template<is_matrix_any_major Mat>
constexpr bool operator!=(const Mat& lhs, const Mat& rhs) noexcept
{
    return !(lhs == rhs);
}

TMATH_NAMESPACE_END
