// #define TMATH_TEST_F16C

#include "benchmark_header.hpp"


using namespace tsimd;

#ifdef _MSC_VER
static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_F16C, use_FMA3, use_AVX2}, { }));
#else
static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_F16C}, {use_AVX2, use_FMA3}));
#endif

