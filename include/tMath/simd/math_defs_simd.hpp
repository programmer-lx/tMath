#pragma once

#include <xmmintrin.h>

#include "../math_defs.hpp"

#define TMATH_SIMD_NAMESPACE_NAME tSimd
#define TMATH_SIMD_NAMESPACE_BEGIN namespace TMATH_SIMD_NAMESPACE_NAME {
#define TMATH_SIMD_NAMESPACE_END }



// SIMD macros
#define TMATH_SIMD_VECTOR_DATA_NAME data


TMATH_SIMD_NAMESPACE_BEGIN

template<typename F>
concept is_float32 = (sizeof(F) == 4);

template<typename F>
concept is_float64 = (sizeof(F) == 8);

namespace detail
{

}

TMATH_SIMD_NAMESPACE_END