// #define TMATH_TEST_FMA3

#include "benchmark_header.hpp"


using namespace tSimd;

#ifdef _MSC_VER
static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_FMA3, use_F16C, use_AVX2}, { }));
#else
static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_FMA3}, {use_AVX2, use_F16C}));
#endif


TMATH_BENCHMARK("float4 mul_add(float4 a, float4 b, float4 c)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 b = next_random_float32_4();
    float32_4 c = next_random_float32_4();
    float32_4 result = _mm_fmadd_ps(a, b, c);
    benchmark::DoNotOptimize(result);
)->Setup(setup_random_float32_4s);


