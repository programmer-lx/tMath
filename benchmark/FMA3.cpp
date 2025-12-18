#define TMATH_TEST_FMA3

#include "benchmark_all.inl"

using namespace tSimd;

static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_FMA3}, {use_AVX2, use_F16C}));

TMATH_BENCHMARK_MAIN();