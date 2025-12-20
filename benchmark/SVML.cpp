#include "benchmark_header.hpp"

using namespace tSimd;

static_assert(test_simd_intrinsics({use_SSE2, use_SVML}, {use_SSE3, use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));

TMATH_BENCHMARK("float4 sin(float4 v)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 result = _mm_sin_ps(a);
    benchmark::DoNotOptimize(result);
)->Setup(setup_random_float32_4s);

TMATH_BENCHMARK("float4 cos(float4 v)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 result = _mm_cos_ps(a);
    benchmark::DoNotOptimize(result);
)->Setup(setup_random_float32_4s);

TMATH_BENCHMARK("float4 tan(float4 v)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 result = _mm_tan_ps(a);
    benchmark::DoNotOptimize(result);
)->Setup(setup_random_float32_4s);
