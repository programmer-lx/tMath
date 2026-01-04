#pragma once

#include "impl/matrix_row_major.hpp"
#include "impl/matrix_column_major.hpp"


#define TMATH_MATRIX_OPERATORS(matrix_type_name) \
    template<TMATH_NAMESPACE_NAME::is_vector_n Vec> \
    constexpr friend Vec operator*(const matrix_type_name& lhs, const Vec& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*(lhs, rhs); } \
    \
    constexpr friend matrix_type_name operator*(const matrix_type_name& lhs, const matrix_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*(lhs, rhs); } \
    \
    constexpr friend bool operator==(const matrix_type_name& lhs, const matrix_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator==(lhs, rhs); }


#define TMATH_FULL_MATRIX4X4_ROW_MAJOR(matrix_type_name, component_type_name)
