#pragma once

#include <tMath/simd/float32_4.hpp>

#include "bm_counters.hpp"
#include "benchmark_utils.hpp"

using namespace tSimd;

static void tmath_benchmark_prepare()
{

}

TMATH_BENCHMARK("float32_4 dot4(float32_4 lhs, float32_4 rhs)", "tMath",
    float32_4 y1 = tSimd::set(1, 2, 3, 4);
    float32_4 y2 = tSimd::set(5, 6, 7, 8);
    float32_4 x = tSimd::dot4(y1, y2);
    benchmark::DoNotOptimize(x);
);