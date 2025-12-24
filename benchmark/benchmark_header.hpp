#pragma once

#include <tMath/simd/float32_4.hpp>
#include <tMath/vector1.hpp>
#include <tMath/vector2.hpp>
#include <tMath/vector3.hpp>
#include <tMath/vector4.hpp>

#include "bm_counters.hpp"
#include "benchmark_utils.hpp"

#if !defined(TMATH_IS_DOING_BENCHMARK) || !defined(TMATH_IS_TESTING)
#error "Please define TMATH_IS_DOING_BENCHMARK and TMATH_IS_TESTING to enable benchmark."
#endif

struct Vector4f
{
    TMATH_FULL_VECTOR4(Vector4f, float)
};

struct Vector4d
{
    TMATH_FULL_VECTOR4(Vector4d, double)
};

struct alignas(16) Scalar_Vector4f_Align16
{
    TMATH_FULL_VECTOR4(Scalar_Vector4f_Align16, float)
};

struct alignas(32) Scalar_Vector4d_Align32
{
    TMATH_FULL_VECTOR4(Scalar_Vector4d_Align32, double)
};
