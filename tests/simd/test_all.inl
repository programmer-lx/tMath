#pragma once

#include <tMath/simd/float4.hpp>

#ifndef TMATH_CHECK_TSIMD_INCLUDE
#error "testing header MUST be included after tSimd library"
#endif

#include "test_float4.inl"

inline void test_all()
{
    test_float4();
}