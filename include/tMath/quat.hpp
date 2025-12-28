#pragma once

#include "math_defs.hpp"

TMATH_NAMESPACE_BEGIN

template<is_quat Q, is_vector4 Vec4>
constexpr Q& operator*=(Q& lhs, const Vec4& rhs) noexcept
{
    // TODO
    return lhs;
}


template<is_quat Q>
constexpr Q& operator*=(Q& lhs, const Q& rhs) noexcept
{
    // TODO
    return lhs;
}



// ============================================= initializers =============================================

template<is_quat Q>
constexpr Q identity() noexcept
{
    using Field = vector_quat_component_t<Q>;
    return { static_cast<Field>(0), static_cast<Field>(0), static_cast<Field>(0), static_cast<Field>(1) };
}

template<is_quat Q, is_vector3 Vec3, is_floating_point F>
constexpr Q axis_angle(const Vec3& axis, const F angle)
{
    // TODO
    return {};
}

TMATH_NAMESPACE_END
