#pragma once

#include "../math_defs.hpp"

TMATH_NAMESPACE_BEGIN

#define TMATH_VECTOR_DATA_INDEX \
    constexpr std::remove_extent_t<decltype(data)>& operator[](int i) { return data[i]; } \
    constexpr const std::remove_extent_t<decltype(data)>& operator[](int i) const { return data[i]; }


// ---------------------------------------------- vec2 ----------------------------------------------


// ---------------------------------------------- vec3 ----------------------------------------------


// ---------------------------------------------- vec4 ----------------------------------------------


// ---------------------------------------------- vecN ----------------------------------------------


TMATH_NAMESPACE_END
