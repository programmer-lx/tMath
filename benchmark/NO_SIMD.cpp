#include <cmath>

#define TMATH_NO_SIMD

#include "benchmark_header.hpp"


using namespace tSimd;


static float cos_taylor_10(float x);

static float sin_taylor_9(float x)
{
    // 先把x映射到 [-PI, PI]
    double val = std::fmod(x, tMath::TwoPI<double>);
    if (val >  tMath::PI<double>) val -= tMath::TwoPI<double>;
    if (val < -tMath::PI<double>) val += tMath::TwoPI<double>;

    // 奇函数，映射到 [0, PI]
    double sign = 1.0;
    if (val < 0.0)
    {
        val *= -1.0;
        sign = -1.0;
    }

    // sin(a) == sin(PI-a)，映射到 [0, PI/2]
    if (val > tMath::HalfPI<double>) val = tMath::PI<double> - val;

    // sin(a) == cos(PI/2 - a)，压缩到 [0, PI/4]
    if (val > tMath::QuarterPI<double>)
    {
        return sign * cos_taylor_10(tMath::HalfPI<double> - val);
    }

    constexpr double c3 = -1.0 / 6.0;
    constexpr double c5 = 1.0 / 120.0;
    constexpr double c7 = -1.0 / 5040.0;
    constexpr double c9 = 1.0 / 362880.0;

    const double x2 = val * val;

    // 泰勒展开，从1阶导数展开到9阶导数，求出近似解
    return static_cast<float>(sign * val * (1.0f + x2 * (c3 + x2 * (c5 + x2 * (c7 + x2 * c9)))));
    // return  val +
    //         ((val * val * val) * c3) +
    //         ((val * val * val * val * val) * c5) +
    //         ((val * val * val * val * val * val * val) * c7) +
    //         ((val * val * val * val * val * val * val * val * val) * c9);
}

static float cos_taylor_10(float x)
{
    // 映射到 [-PI, PI]
    double val = std::fmod(x, tMath::TwoPI<double>);
    if (val >  tMath::PI<double>) val -= tMath::TwoPI<double>;
    if (val < -tMath::PI<double>) val += tMath::TwoPI<double>;

    // 偶函数，映射到 [0, PI]
    if (val < 0.0) val *= -1.0;

    // cos(a) == -cos(PI-a)，映射到 [0, PI/2]
    double sign = 1.0;
    if (val > tMath::HalfPI<double>)
    {
        sign = -1.0;
        val = tMath::PI<double> - val;
    }

    // cos(a) == sin(PI/2 - a)，压缩到 [0, PI/4]
    if (val > tMath::QuarterPI<double>)
    {
        return sign * sin_taylor_9(tMath::HalfPI<double> - val);
    }

    constexpr double c2 = -1.0 / 2.0;
    constexpr double c4 = 1.0 / 24.0;
    constexpr double c6 = -1.0 / 720.0;
    constexpr double c8 = 1.0 / 40'320.0;
    constexpr double c10 = -1.0 / 3'628'800.0;

    const double x2 = val * val;
    return static_cast<float>(sign * (1.0 + (x2 * (c2 + x2 * (c4 + x2 * (c6 + x2 * (c8 + x2 * c10)))))));
    // return  1.0 +
    //         (val * val * c2) +
    //         (val * val * val * val * c4) +
    //         (val * val * val * val * val * val * c6) +
    //         (val * val * val * val * val * val * val * val * c8) +
    //         (val * val * val * val * val * val * val * val * val * val * c10);
}




static_assert(test_simd_intrinsics({}, {use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));

TMATH_BENCHMARK("float sin(float x)", "使用9阶泰勒展开的sin实现 (测试自己实现的算法与标准库的性能差距)", 512,
    float x = next_random_float();
    benchmark::DoNotOptimize(sin_taylor_9(x));
)->Setup(setup_random_floats);

TMATH_BENCHMARK("float sin(float x)", "std::sin", 512,
    float x = next_random_float();
    benchmark::DoNotOptimize(std::sin(x));
)->Setup(setup_random_floats);




TMATH_BENCHMARK("float cos(float x)", "使用10阶泰勒展开的cos实现 (测试自己实现的算法与标准库的性能差距)", 512,
    float x = next_random_float();
    benchmark::DoNotOptimize(cos_taylor_10(x));
)->Setup(setup_random_floats);

TMATH_BENCHMARK("float cos(float x)", "std::cos", 512,
    float x = next_random_float();
    benchmark::DoNotOptimize(std::cos(x));
)->Setup(setup_random_floats);





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
