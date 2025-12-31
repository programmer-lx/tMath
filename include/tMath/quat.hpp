#pragma once

#include "math_defs.hpp"
#include "impl/fwd_vector.hpp"

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
    using Field = quat_component_t<Q>;
    return { static_cast<Field>(0), static_cast<Field>(0), static_cast<Field>(0), static_cast<Field>(1) };
}

template<is_quat Q, is_vector3 Vec3, is_floating_point F>
constexpr Q axis_angle(const Vec3& axis, const F angle)
{
    // TODO
    return {};
}




// ...参数是 Quat 类型全名
#define TMATH_GENERIC_QUAT_OPERATORS(...)

#define TMATH_QUAT_OPERATORS(quat_type_name) TMATH_GENERIC_QUAT_OPERATORS(quat_type_name)

#define TMATH_FULL_QUAT_BUILTIN(quat_type_name, component_type_name) \
    TMATH_QUAT_TAG \
    union \
    { \
        component_type_name data[4]; \
        struct { component_type_name x, y, z, w; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX

#define TMATH_FULL_QUAT(quat_type_name, component_type_name) \
    TMATH_FULL_QUAT_BUILTIN(quat_type_name, component_type_name) \
    TMATH_GENERIC_QUAT_OPERATORS(quat_type_name)


TMATH_NAMESPACE_END
