#pragma once

#include <cmath>

#include "../math_defs.hpp"

TMATH_NAMESPACE_BEGIN

// ============================================= functions =============================================
template<is_number N1, is_number N2, is_number N3, is_number N4, is_number N5, is_number N6, is_number N7, is_number N8>
constexpr auto dot(
    const N1 x1, const N2 y1, const N3 z1, const N4 w1,
    const N5 x2, const N6 y2, const N7 z2, const N8 w2
) noexcept
{
    using ret_t = std::common_type_t<
        number_to_floating_point_t<N1>,
        number_to_floating_point_t<N2>,
        number_to_floating_point_t<N3>,
        number_to_floating_point_t<N4>,
        number_to_floating_point_t<N5>,
        number_to_floating_point_t<N6>,
        number_to_floating_point_t<N7>,
        number_to_floating_point_t<N8>
    >;

    return static_cast<ret_t>(  static_cast<ret_t>(x1) * static_cast<ret_t>(x2) +
                                static_cast<ret_t>(y1) * static_cast<ret_t>(y2) +
                                static_cast<ret_t>(z1) * static_cast<ret_t>(z2) +
                                static_cast<ret_t>(w1) * static_cast<ret_t>(w2)
                                );
}

// TODO vector3's cross, ignore w component, the return value's w is zero
template<is_vector4 Vec4>
constexpr Vec4 cross(const Vec4& lhs, const Vec4& rhs) noexcept
{
    return {
        lhs.data[1] * rhs.data[2] - lhs.data[2] * rhs.data[1],
        lhs.data[2] * rhs.data[0] - lhs.data[0] * rhs.data[2],
        lhs.data[0] * rhs.data[1] - lhs.data[1] * rhs.data[0],
        static_cast<vector_component_t<Vec4>>(0)
    };
}

TMATH_NAMESPACE_END
