// #define TMATH_TEST_SSE4_1

#include "benchmark_header.hpp"


using namespace tsimd;

#ifdef _MSC_VER
static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX}, {use_AVX2, use_FMA3, use_F16C}));
#else
static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1}, {use_AVX, use_AVX2, use_FMA3, use_F16C}));
#endif

TMATH_BENCHMARK("float4 dot4(float4 lhs, float4 rhs)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 b = next_random_float32_4();

    float32_4 result = _mm_dp_ps(a, b, 0xff);
    benchmark::DoNotOptimize(result);

)->Setup(setup_random_float32_4s);