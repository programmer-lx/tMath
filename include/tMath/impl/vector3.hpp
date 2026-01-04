#pragma once

#include <cmath>

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

// ============================================= functions =============================================

template<is_number N1, is_number N2, is_number N3, is_number N4, is_number N5, is_number N6>
constexpr auto dot(
    const N1 x1, const N2 y1, const N3 z1,
    const N4 x2, const N5 y2, const N6 z2
) noexcept
{
    using ret_t = std::common_type_t<
        number_to_floating_point_t<N1>,
        number_to_floating_point_t<N2>,
        number_to_floating_point_t<N3>,
        number_to_floating_point_t<N4>,
        number_to_floating_point_t<N5>,
        number_to_floating_point_t<N6>
    >;

    return static_cast<ret_t>(  static_cast<ret_t>(x1) * static_cast<ret_t>(x2) +
                                static_cast<ret_t>(y1) * static_cast<ret_t>(y2) +
                                static_cast<ret_t>(z1) * static_cast<ret_t>(z2));
}

template<is_vector3 Vec3>
constexpr Vec3 cross(const Vec3& lhs, const Vec3& rhs) noexcept
{
    return {
        lhs.data[1] * rhs.data[2] - lhs.data[2] * rhs.data[1],
        lhs.data[2] * rhs.data[0] - lhs.data[0] * rhs.data[2],
        lhs.data[0] * rhs.data[1] - lhs.data[1] * rhs.data[0]
    };
}

TMATH_NAMESPACE_END
