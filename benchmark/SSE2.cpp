// #define TMATH_TEST_SSE2

#include "benchmark_header.hpp"


using namespace tSimd;

static_assert(test_simd_intrinsics({use_SSE2}, {use_SSE3, use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));


TMATH_BENCHMARK("float4 mul_add(float4 a, float4 b, float4 c)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 b = next_random_float32_4();
    float32_4 c = next_random_float32_4();
    float32_4 result = _mm_add_ps(_mm_mul_ps(a, b), c);
    benchmark::DoNotOptimize(result);
)->Setup(setup_random_float32_4s);


TMATH_BENCHMARK("float4 dot4(float4 lhs, float4 rhs)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 b = next_random_float32_4();

    float32_4 t1 = _mm_mul_ps(a, b);                                // [w1w2, z1z2, y1y2, x1x2]
    float32_4 t2 = _mm_movehl_ps(t1, t1);                           // [?, ?, w1w2, z1z2]
    float32_4 t3 = _mm_add_ps(t1, t2);                              // [?, ?, y1y2 + w1w2, x1x2 + z1z2]
    float32_4 t4 = tmath_permute_ps(t3, _MM_SHUFFLE(1, 1, 1, 1));   // [?, ?, ?, y1y2 + w1w2]
    float32_4 t5 = _mm_add_ss(t3, t4);                              // [?, ?, ?, x1x2 + z1z2 + y1y2 + w1w2]
    float32_4 result = tmath_permute_ps(t5, _MM_SHUFFLE(0, 0, 0, 0));
    benchmark::DoNotOptimize(result);

)->Setup(setup_random_float32_4s);