#define TMATH_NO_SIMD

#include "benchmark_header.hpp"


using namespace tSimd;

static_assert(test_simd_intrinsics({}, {use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));

TMATH_BENCHMARK("float4 mul_add(float4 a, float4 b, float4 c)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 b = next_random_float32_4();
    float32_4 c = next_random_float32_4();
    float32_4 m = {
        a.x * b.x,
        a.y * b.y,
        a.z * b.z,
        a.w * b.w
    };
    float32_4 result = m + c;
    benchmark::DoNotOptimize(result);
)->Setup(setup_random_float32_4s);

TMATH_BENCHMARK("float4 dot4(float4 lhs, float4 rhs)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 b = next_random_float32_4();

    float value = TMATH_NAMESPACE_NAME::dot(a, b);
    float32_4 result = { value, value, value, value };
    benchmark::DoNotOptimize(result);

)->Setup(setup_random_float32_4s);

TMATH_BENCHMARK("float4 sin(float4 v)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 result = TMATH_NAMESPACE_NAME::sin(a);
    benchmark::DoNotOptimize(result);
)->Setup(setup_random_float32_4s);

TMATH_BENCHMARK("float4 cos(float4 v)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 result = TMATH_NAMESPACE_NAME::cos(a);
    benchmark::DoNotOptimize(result);
)->Setup(setup_random_float32_4s);

TMATH_BENCHMARK("float4 tan(float4 v)", "", 512,
    float32_4 a = next_random_float32_4();
    float32_4 result = TMATH_NAMESPACE_NAME::tan(a);
    benchmark::DoNotOptimize(result);
)->Setup(setup_random_float32_4s);
