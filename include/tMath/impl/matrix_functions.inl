#pragma once

#include <utility>

#include "../math_defs.hpp"
#include "../vector1.hpp"
#include "../vector2.hpp"
#include "../vector3.hpp"
#include "../vector4.hpp"
#include "../large_vector.hpp"

TMATH_NAMESPACE_BEGIN

namespace detail
{
    template<is_square_matrix_any_major Mat, size_t I, size_t... J, is_signed_number N>
    constexpr matrix_traits<Mat>::vector_type scale_i_impl(const N scalar, std::index_sequence<J...>) noexcept
    {
        using C = matrix_traits<Mat>::component_type;
        return { static_cast<C>(I == J) * static_cast<C>(scalar)... };
    }

    template<is_square_matrix_any_major Mat, size_t... I, is_signed_number N>
    constexpr Mat scale_impl(const N scalar, std::index_sequence<I...>) noexcept
    {
        return { scale_i_impl<Mat, I>(scalar, std::make_index_sequence<matrix_traits<Mat>::vector_component_count>{})... };
    }
} // namespace detail

/**
 * 缩放矩阵
 */
template<is_square_matrix_any_major Mat, is_signed_number N>
constexpr Mat scale(const N scalar) noexcept
{
    return detail::scale_impl<Mat>(scalar, std::make_index_sequence<matrix_traits<Mat>::vector_count>{});
}

/**
 * 单位矩阵
 */
template<is_square_matrix_any_major Mat>
constexpr Mat identity() noexcept
{
    return scale<Mat>(1);
}


namespace detail
{
    template<is_square_matrix_any_major Mat, size_t I, size_t... J>
    constexpr matrix_traits<Mat>::vector_type transpose_i_impl(const Mat& m, std::index_sequence<J...>) noexcept
    {
        return { m.data[J].data[I]... };
    }

    template<is_square_matrix_any_major Mat, size_t... I>
    constexpr Mat transpose_impl(const Mat& m, std::index_sequence<I...>) noexcept
    {
        return { transpose_i_impl<Mat, I>(m, std::make_index_sequence<matrix_traits<Mat>::vector_component_count>{})... };
    }
}
template<is_square_matrix_any_major Mat>
constexpr Mat transpose(const Mat& m) noexcept
{
    return detail::transpose_impl(m, std::make_index_sequence<matrix_traits<Mat>::vector_count>{});
}


TMATH_NAMESPACE_END
