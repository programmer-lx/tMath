#pragma once

#include <tMath/simd/float32_4.hpp>

#include "bm_counters.hpp"
#include "benchmark_utils.hpp"

static void tmath_benchmark_prepare()
{

}

TMATH_BENCHMARK("float32_4 test_name(float32_4 lhs, float32_4 rhs)", "tMath",
    float x = std::sin(6);
    benchmark::DoNotOptimize(x);
);

TMATH_BENCHMARK("float32_4 dot4(float32_4 lhs, float32_4 rhs)", "tMath",
    float x = std::sin(6);
    benchmark::DoNotOptimize(x);
);