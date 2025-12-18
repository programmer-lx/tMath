#include <tMath/vector1.hpp>

#include <gtest/gtest.h>

#include "test.hpp"

TEST(vector1, approximately)
{
    EXPECT_TRUE(tMath::approximately(1.0000005, 1.000001f, 0.000001f));
    EXPECT_TRUE(tMath::approximately(1.0000005, 1.000001, 0.000001));
}

TEST(vector1, to_radians)
{
    EXPECT_TRUE(tMath::approximately(tMath::to_radians(180.0f), tMath::PI<float>));
}

TEST(vector1, degrees)
{
    EXPECT_TRUE(tMath::approximately(tMath::to_degrees(tMath::PI<float>), 180.0f));
}

TEST(vector1, abs)
{
    EXPECT_TRUE(tMath::approximately(tMath::abs(-1.111f), 1.111f));
    EXPECT_TRUE(tMath::abs(-1) == 1);
}

TEST(vector1, clamp)
{
    float a = 10;
    EXPECT_TRUE(tMath::clamp(a, 0.0f, tMath::Epsilon<float>) == tMath::Epsilon<float>);
}

TEST(vector1, lerp)
{
    EXPECT_TRUE(tMath::approximately(tMath::lerp(-10.0f, 10.0f, 0.6), 2.0f));
}

TEST(vector1, triangle_functions)
{
    float x = -0.1896513f;
    float y = 0.456987f;
    EXPECT_TRUE(tMath::approximately(tMath::sin(x), sin(x)));
    EXPECT_TRUE(tMath::approximately(tMath::asin(x), asin(x)));
    EXPECT_TRUE(tMath::approximately(tMath::cos(x), cos(x)));
    EXPECT_TRUE(tMath::approximately(tMath::acos(x), acos(x)));
    EXPECT_TRUE(tMath::approximately(tMath::tan(x), tan(x)));
    EXPECT_TRUE(tMath::approximately(tMath::atan(x), atan(x)));
    EXPECT_TRUE(tMath::approximately(tMath::atan2(y, x), atan2(y, x)));
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



TMATH_TEST_MAIN()
