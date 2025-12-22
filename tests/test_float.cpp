#include <tMath/vector1.hpp>

#include "test.hpp"


TEST(vector1, approximately)
{
    EXPECT_TRUE(tMath::approximately(1.0000005, 1.000001f, 0.000001f));
    EXPECT_TRUE(tMath::approximately(1.0000005, 1.000001, 0.000001));
}

TEST(vector1, to_radians)
{
    constexpr float result = tMath::to_radians(180.0f);
    EXPECT_TRUE(tMath::approximately(result, tMath::PI<float>));
}

TEST(vector1, factorial)
{
    EXPECT_EQ(tMath::factorial(10), 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
    EXPECT_EQ(tMath::factorial(11), 11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
    EXPECT_EQ(tMath::factorial(-1), 1);
    EXPECT_EQ(tMath::factorial(0), 1);
}

TEST(vector1, degrees)
{
    constexpr float result = tMath::to_degrees(tMath::PI<float>);
    EXPECT_TRUE(tMath::approximately(result, 180.0f));
}

TEST(vector1, exp)
{
    EXPECT_EQ(tMath::exp(5.0), std::exp(5.0));
    EXPECT_EQ(tMath::exp(5.0f), std::exp(5.0f));
}

TEST(vector1, min)
{
    EXPECT_EQ(tMath::min(1, 2), std::min(1, 2));
}

TEST(vector1, max)
{
    EXPECT_EQ(tMath::max(1, 2), std::max(1, 2));
}

TEST(vector1, abs)
{
    EXPECT_TRUE(tMath::approximately(tMath::abs(-1.111f), 1.111f));
    EXPECT_TRUE(tMath::abs(-1) == 1);
}

TEST(vector1, clamp)
{
    constexpr float a = 10;
    constexpr float result = tMath::clamp(a, 0.0f, tMath::Epsilon<float>);
    EXPECT_TRUE(result == tMath::Epsilon<float>);

    constexpr float result2 = tMath::clamp(tMath::NaN<float>, 1.f, 2.f);
    EXPECT_TRUE(tMath::is_nan(result2));
}

TEST(vector1, lerp)
{
    constexpr float result = tMath::lerp(-10.0f, 10.0f, 0.6);
    EXPECT_TRUE(tMath::approximately(result, 2.0f));
}

TEST(vector1, lerp_saturated)
{
    constexpr float r1 = tMath::lerp_saturated(-10.0f, 10.0f, 1.2f);
    EXPECT_TRUE(tMath::approximately(r1, 10.0f));

    constexpr float r2 = tMath::lerp_saturated(-10.0f, 10.0f, -2.f);
    EXPECT_TRUE(tMath::approximately(r2, -10.0f));
}

TEST(vector1, min_max)
{
    {
        // min
        constexpr float result = tMath::min(1, 2);
        EXPECT_EQ(result, 1);
    }
    {
        // max
        constexpr float result = tMath::max(2, 3);
        EXPECT_EQ(result, 3);
    }
}

TEST(vector1, triangle_functions)
{
    float x = -0.1896513f;
    float y = 0.456987f;
    EXPECT_TRUE(tMath::approximately(tMath::sin(x), std::sin(x)));
    EXPECT_TRUE(tMath::approximately(tMath::asin(x), std::asin(x)));
    EXPECT_TRUE(tMath::approximately(tMath::cos(x), std::cos(x)));
    EXPECT_TRUE(tMath::approximately(tMath::acos(x), std::acos(x)));
    EXPECT_TRUE(tMath::approximately(tMath::tan(x), std::tan(x)));
    EXPECT_TRUE(tMath::approximately(tMath::atan(x), std::atan(x)));
    EXPECT_TRUE(tMath::approximately(tMath::atan2(y, x), std::atan2(y, x)));
}

TEST(vector1, zero_divide)
{
    // zero divide
    {
        int t = 0;
        float f = 10.0 / t;
        EXPECT_TRUE(tMath::is_infinity(f));
    }
    {
        int t = 0;
        float f = 0.0 / t;
        EXPECT_TRUE(tMath::is_nan(f));
    }
    {
        float f = 1;
        float zero = 0;
        EXPECT_TRUE(tMath::safe_divide(f, zero, 100.0f) == 100.0f);
    }
    {
        float f = 1;
        float zero = 0;
        tMath::safe_divide_inplace(f, zero, 666.0f);
        EXPECT_TRUE(f == 666.0f);
    }
}



