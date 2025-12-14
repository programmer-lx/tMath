#pragma once

#include <cmath>

#include <tMath/math_defs.hpp>

TMATH_NAMESPACE_BEGIN

template<is_vector3_f32 TVec3>
vector_field_t<TVec3> magnitude(const TVec3& v)
{
    return std::hypot(v.x, v.y, v.z);
}

TMATH_NAMESPACE_END