#pragma once

#include <utility>

#include "math_defs.hpp"
#include "../vector.hpp"

TMATH_NAMESPACE_BEGIN


// ============================================= operators =============================================
namespace detail
{
    template<is_matrix_row_major Mat, is_vector_n Vec, size_t... I>
    constexpr Vec operator_mul_impl(const Mat& lhs, const Vec& rhs, std::index_sequence<I...>) noexcept
    {
        return { TMATH_NAMESPACE_NAME::dot(lhs.data[I], rhs)... };
    }
}
template<is_matrix_row_major Mat, is_vector_n Vec>
constexpr Vec operator*(const Mat& lhs, const Vec& rhs) noexcept
{
    static_assert(matrix_traits<Mat>::column_count == vector_traits<Vec>::component_count);

    return detail::operator_mul_impl(lhs, rhs, std::make_index_sequence<matrix_traits<Mat>::vector_count>{});
}


namespace detail
{
    template<is_square_matrix_row_major Mat, size_t Row, size_t Col, size_t... K>
    constexpr auto mat_dot_impl(const Mat& lhs, const Mat& rhs, std::index_sequence<K...>) noexcept
    {
        return ( (lhs.data[Row].data[K] * rhs.data[K].data[Col]) + ... );
    }

    template<is_square_matrix_row_major Mat, size_t Row, size_t... Col>
    constexpr matrix_traits<Mat>::vector_type mul_row_impl(const Mat& lhs, const Mat& rhs, std::index_sequence<Col...>) noexcept
    {
        return { mat_dot_impl<Mat, Row, Col>(lhs, rhs, std::make_index_sequence<matrix_traits<Mat>::row_count>{})... };
    }

    template<is_square_matrix_row_major Mat, size_t... Row>
    constexpr Mat operator_mul_impl(const Mat& lhs, const Mat& rhs, std::index_sequence<Row...>) noexcept
    {
        return { mul_row_impl<Mat, Row>(lhs, rhs, std::make_index_sequence<matrix_traits<Mat>::column_count>{})... };
    }
}
template<is_square_matrix_row_major Mat>
constexpr Mat operator*(const Mat& lhs, const Mat& rhs) noexcept
{
    constexpr int N = matrix_traits<Mat>::row_count;
    return detail::operator_mul_impl(lhs, rhs, std::make_index_sequence<N>{});
}


TMATH_NAMESPACE_END
#include "matrix_operators.inl"
TMATH_NAMESPACE_BEGIN


// ============================================= casts =============================================
TMATH_NAMESPACE_END
#include "matrix_casts.inl"
TMATH_NAMESPACE_BEGIN


// ============================================= functions =============================================



TMATH_NAMESPACE_END
#include "matrix_functions.inl"
TMATH_NAMESPACE_BEGIN

TMATH_NAMESPACE_END
