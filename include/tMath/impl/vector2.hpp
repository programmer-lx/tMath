#pragma once

#include <cmath>

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN


// ============================================= functions =============================================

template<is_number N1, is_number N2, is_number N3, is_number N4>
constexpr auto dot(
    const N1 x1, const N2 y1,
    const N3 x2, const N4 y2
) noexcept
{
    using ret_t = std::common_type_t<
        number_to_floating_point_t<N1>,
        number_to_floating_point_t<N2>,
        number_to_floating_point_t<N3>,
        number_to_floating_point_t<N4>
    >;

    return static_cast<ret_t>(static_cast<ret_t>(x1) * static_cast<ret_t>(x2) + static_cast<ret_t>(y1) * static_cast<ret_t>(y2));
}

template<is_number N1, is_number N2, is_number N3, is_number N4>
constexpr auto cross(
    const N1 x1, const N2 y1,
    const N3 x2, const N4 y2
) noexcept
{
    using ret_t = std::common_type_t<
        number_to_floating_point_t<N1>,
        number_to_floating_point_t<N2>,
        number_to_floating_point_t<N3>,
        number_to_floating_point_t<N4>
    >;

    return static_cast<ret_t>(static_cast<ret_t>(x1) * static_cast<ret_t>(y2) - static_cast<ret_t>(x2) * static_cast<ret_t>(y1));
}

template<is_vector2 Vec2>
constexpr vector_component_t<Vec2> cross(const Vec2& lhs, const Vec2& rhs) noexcept
{
    using ret_t = vector_component_t<Vec2>;

    return static_cast<ret_t>(lhs.data[0] * rhs.data[1] - lhs.data[1] * rhs.data[0]);
}

TMATH_NAMESPACE_END
