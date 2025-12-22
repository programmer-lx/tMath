#include <format>

#include "test.hpp"

#include <tMath/vector1.hpp>

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
        return static_cast<float>(sign * cos_taylor_10(static_cast<float>(tMath::HalfPI<double> - val)));
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

TEST(algorithm, sin_taylor_9)
{
    for (int i = 0; i < 500; ++i)
    {
        float v = random_f(-100.0f, 100.0f);
        float result = sin_taylor_9(v);
        float test = std::sin(v);
        EXPECT_TRUE(tMath::approximately(result, test, tMath::Epsilon<float> * 1.0f)) << std::format("result: {}, test: {}", result, test);
    }
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
        return static_cast<float>(sign * sin_taylor_9(static_cast<float>(tMath::HalfPI<double> - val)));
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

TEST(algorithm, cos_taylor_10)
{
    for (int i = 0; i < 500; ++i)
    {
        float v = random_f(-100.0f, 100.0f);
        float result = cos_taylor_10(v);
        float test = std::cos(v);
        EXPECT_TRUE(tMath::approximately(result, test, tMath::Epsilon<float> * 1.0f)) << std::format("result: {}, test: {}", result, test);
    }
}
