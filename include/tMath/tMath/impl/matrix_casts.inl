#pragma once

#include <utility>

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

namespace detail
{
    template<is_matrix_any_major Ret, is_matrix_any_major In, size_t I, size_t... J>
    constexpr matrix_traits<Ret>::vector_type matrix_cast_i_impl(const In& m, std::index_sequence<J...>) noexcept
    {
        return { static_cast<matrix_traits<Ret>::component_type>(m.data[I].data[J])... };
    }

    template<is_matrix_any_major Ret, is_matrix_any_major In, size_t... I>
    constexpr Ret matrix_cast_impl(const In& m, std::index_sequence<I...>) noexcept
    {
        return { matrix_cast_i_impl<Ret, In, I>(m, std::make_index_sequence<matrix_traits<Ret>::vector_component_count>{})... };
    }
}
template<is_matrix_any_major Ret, is_matrix_any_major In>
constexpr Ret matrix_cast(const In& m) noexcept
{
    // 逻辑行列要相等
    static_assert(  matrix_traits<Ret>::column_count == matrix_traits<In>::column_count &&
                    matrix_traits<Ret>::row_count == matrix_traits<In>::row_count,
                    "Ret matrix and In matrix MUST have the same column count and row count");

    if constexpr (matrix_traits<Ret>::is_column_major == matrix_traits<In>::is_column_major)
    {
        // 相同主序
        return detail::matrix_cast_impl<Ret>(m, std::make_index_sequence<matrix_traits<Ret>::vector_count>{});
    }
    else
    {
        // 不同主序
        // 相当于是转置之后再返回
        TMATH_ASSERT(false && "TODO");
        return {};
    }
}

TMATH_NAMESPACE_END
