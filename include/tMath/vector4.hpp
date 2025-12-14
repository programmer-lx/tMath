#pragma once

#include <cmath>

#include "math_defs.hpp"
#include "vector1.hpp"

TMATH_NAMESPACE_BEGIN

template<is_vector4 TVec4>
vector_field_t<TVec4> magnitude(const TVec4& v)
{
    return TMATH_NAMESPACE_NAME::sqrt(
        v.x * v.x + 
        v.y * v.y + 
        v.z * v.z + 
        v.w * v.w
    );
}

TMATH_NAMESPACE_END