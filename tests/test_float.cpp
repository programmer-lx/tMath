#include <vector>
#include <climits> // 用于 CHAR_BIT
#include <cstring>

#include <tMath/number.hpp>

#include "test.hpp"

TEST(float_boundary, inf)
{
    volatile float inf = tMath::Infinity<float>;
    volatile float epsilon = tMath::Epsilon<float>;

    // +
    {
        EXPECT_TRUE(inf + 5.0f == inf);
        EXPECT_TRUE(tMath::is_infinity(inf + 5.0f));

        EXPECT_TRUE(5.0f + inf == inf);
        EXPECT_TRUE(tMath::is_infinity(5.0f + inf));

        EXPECT_TRUE(-5.0f + inf == inf);
        EXPECT_TRUE(tMath::is_infinity(-5.0f + inf));

        EXPECT_TRUE(inf + inf == inf);
        EXPECT_TRUE(tMath::is_infinity(inf + inf));
    }

    // -
    {
        EXPECT_TRUE(inf - 5.0f == inf);
        EXPECT_TRUE(tMath::is_infinity(inf - 5.0f));

        EXPECT_TRUE(5.0f - inf == -inf);
        EXPECT_TRUE(tMath::is_infinity(5.0f - inf));

        EXPECT_TRUE(-5.0f - inf == -inf);
        EXPECT_TRUE(tMath::is_infinity(-5.0f - inf));

        EXPECT_TRUE(tMath::is_nan(inf - inf));
    }

    // *
    {
        // 有限数 * 有限数 溢出测试
        EXPECT_TRUE(inf == inf);
        EXPECT_TRUE(tMath::Max<float> * tMath::Max<float> == inf);

        EXPECT_TRUE(inf * 10 == inf);
        EXPECT_TRUE(tMath::is_infinity(inf * 10));

        EXPECT_TRUE(inf * -10 == -inf);
        EXPECT_TRUE(tMath::is_infinity(inf * -10));

        EXPECT_TRUE(tMath::is_nan(inf * 0.0f));
        EXPECT_TRUE(tMath::is_nan(inf * 0));

        EXPECT_TRUE(1e32f * 1e32f == inf);
        EXPECT_TRUE(1e32f * -1e32f == -inf);
        EXPECT_TRUE(-1e32f * 1e32f == -inf);

        EXPECT_TRUE(inf * inf == inf);
        EXPECT_TRUE(tMath::is_infinity(inf * inf));
    }

    // /
    {
        EXPECT_TRUE((1.0f / inf) == 0.0f); // inf符号符合算术规则
        TMATH_EXPECT_IS_POSITIVE(1.0f / inf);
        EXPECT_TRUE((0.0f / inf) == 0.0f);
        TMATH_EXPECT_IS_POSITIVE(0.0f / inf);
        EXPECT_TRUE((-5.0f / inf) == 0.0f);
        TMATH_EXPECT_IS_NEGATIVE(-5.0f / inf);

        EXPECT_TRUE((1.0f / -inf) == 0.0f);
        TMATH_EXPECT_IS_NEGATIVE(1.0f / -inf);
        EXPECT_TRUE((0.0f / -inf) == 0.0f);
        TMATH_EXPECT_IS_NEGATIVE(0.0f / -inf);
        EXPECT_TRUE((-5.0f / -inf) == 0.0f);
        TMATH_EXPECT_IS_POSITIVE(-5.0f / -inf);

        EXPECT_TRUE(tMath::is_nan(inf / inf));
        EXPECT_TRUE((inf / 5.0f) == inf);
        EXPECT_TRUE((inf / -5.0f) == -inf);

        EXPECT_TRUE(std::sqrt(inf) == inf);
    }

    // >
    {
        EXPECT_TRUE((inf > epsilon) == true);
        EXPECT_TRUE((inf > -epsilon) == true);
        EXPECT_TRUE((inf > 1) == true);
        EXPECT_TRUE((inf > -1) == true);
        EXPECT_TRUE((-inf > -1) == false);
        EXPECT_TRUE((-inf > 1) == false);
    }
}

TEST(float_boundary, nan)
{
    volatile float nan = tMath::QuietNaN<float>;
    volatile float inf = tMath::Infinity<float>;
    volatile float epsilon = tMath::Epsilon<float>;

    static_assert(std::numeric_limits<float>::is_iec559);

    // +
    {
        EXPECT_TRUE(tMath::is_nan(nan + 5));
        EXPECT_TRUE(tMath::is_nan(5 + nan));
        EXPECT_TRUE(tMath::is_nan(-5 + nan));
        EXPECT_TRUE(tMath::is_nan(0.0f + nan));
        EXPECT_TRUE(tMath::is_nan(-0.0f + nan));
        EXPECT_TRUE(tMath::is_nan(inf + nan));
        EXPECT_TRUE(tMath::is_nan(-inf + nan));
        EXPECT_TRUE(tMath::is_nan(nan + nan));
    }

    // -
    {
        EXPECT_TRUE(tMath::is_nan(nan - 5));
        EXPECT_TRUE(tMath::is_nan(5 - nan));
        EXPECT_TRUE(tMath::is_nan(-5 - nan));
        EXPECT_TRUE(tMath::is_nan(0.0f - nan));
        EXPECT_TRUE(tMath::is_nan(-0.0f - nan));
        EXPECT_TRUE(tMath::is_nan(inf - nan));
        EXPECT_TRUE(tMath::is_nan(-inf - nan));
        EXPECT_TRUE(tMath::is_nan(nan - nan));
    }

    // *
    {
        EXPECT_TRUE(tMath::is_nan(nan * 5));
        EXPECT_TRUE(tMath::is_nan(5 * nan));
        EXPECT_TRUE(tMath::is_nan(-5 * nan));
        EXPECT_TRUE(tMath::is_nan(0.0f * nan));
        EXPECT_TRUE(tMath::is_nan(-0.0f * nan));
        EXPECT_TRUE(tMath::is_nan(inf * nan));
        EXPECT_TRUE(tMath::is_nan(-inf * nan));
        EXPECT_TRUE(tMath::is_nan(nan * nan));
    }

    // /
    {
        EXPECT_TRUE(tMath::is_nan(nan / 5));
        EXPECT_TRUE(tMath::is_nan(5 / nan));
        EXPECT_TRUE(tMath::is_nan(-5 / nan));
        EXPECT_TRUE(tMath::is_nan(0.0f / nan));
        EXPECT_TRUE(tMath::is_nan(-0.0f / nan));
        EXPECT_TRUE(tMath::is_nan(inf / nan));
        EXPECT_TRUE(tMath::is_nan(-inf / nan));
        EXPECT_TRUE(tMath::is_nan(nan / nan));
    }

    // <
    {
        EXPECT_TRUE((nan < 5) == false);
        EXPECT_TRUE((nan < -5) == false);
        EXPECT_TRUE((nan < 0.0f) == false);
        EXPECT_TRUE((nan < -0.0f) == false);
        EXPECT_TRUE((nan < nan) == false);
        EXPECT_TRUE((nan < inf) == false);
        EXPECT_TRUE((nan < -inf) == false);
        EXPECT_TRUE((nan < epsilon) == false);
        EXPECT_TRUE((nan < -epsilon) == false);

        EXPECT_TRUE((5 < nan) == false);
        EXPECT_TRUE((-5 < nan) == false);
        EXPECT_TRUE((0.0f < nan) == false);
        EXPECT_TRUE((-0.0f < nan) == false);
        EXPECT_TRUE((nan < nan) == false);
        EXPECT_TRUE((inf < nan) == false);
        EXPECT_TRUE((-inf < nan) == false);
        EXPECT_TRUE((epsilon < nan) == false);
        EXPECT_TRUE((-epsilon < nan) == false);
    }

    // >
    {
        EXPECT_TRUE((nan > 5) == false);
        EXPECT_TRUE((nan > -5) == false);
        EXPECT_TRUE((nan > 0.0f) == false);
        EXPECT_TRUE((nan > -0.0f) == false);
        EXPECT_TRUE((nan > nan) == false);
        EXPECT_TRUE((nan > inf) == false);
        EXPECT_TRUE((nan > -inf) == false);

        EXPECT_TRUE((5 > nan) == false);
        EXPECT_TRUE((-5 > nan) == false);
        EXPECT_TRUE((0.0f > nan) == false);
        EXPECT_TRUE((-0.0f > nan) == false);
        EXPECT_TRUE((nan > nan) == false);
        EXPECT_TRUE((inf > nan) == false);
        EXPECT_TRUE((-inf > nan) == false);

        EXPECT_TRUE(std::isgreater(nan, inf) == false);
    }

    // &&
    {
        EXPECT_TRUE((nan && 1) == true);
        EXPECT_TRUE((nan && 0) == false);
        EXPECT_TRUE((nan && epsilon) == true);
    }

    // ==
    {
        EXPECT_TRUE((nan == 5) == false);
        EXPECT_TRUE((nan == -5) == false);
        EXPECT_TRUE((nan == 0.0f) == false);
        EXPECT_TRUE((nan == -0.0f) == false);
        EXPECT_TRUE((nan == nan) == false);
        EXPECT_TRUE((nan == inf) == false);
        EXPECT_TRUE((nan == -inf) == false);

        EXPECT_TRUE((5 == nan) == false);
        EXPECT_TRUE((-5 == nan) == false);
        EXPECT_TRUE((0.0f == nan) == false);
        EXPECT_TRUE((-0.0f == nan) == false);
        EXPECT_TRUE((nan == nan) == false);
        EXPECT_TRUE((inf == nan) == false);
        EXPECT_TRUE((-inf == nan) == false);
    }

    // !=
    {
        EXPECT_TRUE((nan != 5) == true);
        EXPECT_TRUE((nan != -5) == true);
        EXPECT_TRUE((nan != 0.0f) == true);
        EXPECT_TRUE((nan != -0.0f) == true);
        EXPECT_TRUE((nan != nan) == true);
        EXPECT_TRUE((nan != inf) == true);
        EXPECT_TRUE((nan != -inf) == true);

        EXPECT_TRUE((5 != nan) == true);
        EXPECT_TRUE((-5 != nan) == true);
        EXPECT_TRUE((0.0f != nan) == true);
        EXPECT_TRUE((-0.0f != nan) == true);
        EXPECT_TRUE((nan != nan) == true);
        EXPECT_TRUE((inf != nan) == true);
        EXPECT_TRUE((-inf != nan) == true);
    }
}

TEST(approximately, normal)
{
    EXPECT_TRUE(tMath::approximately(1.0000005, 1.000001f, 0.000001f));
    EXPECT_TRUE(tMath::approximately(1.0000005, 1.000001, 0.000001));
}

TEST(to_radians, normal)
{
    constexpr float result = tMath::to_radians(180.0f);
    EXPECT_TRUE(tMath::approximately(result, tMath::PI<float>));
}

TEST(factorial, normal)
{
    EXPECT_EQ(tMath::factorial(10), 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
    EXPECT_EQ(tMath::factorial(11), 11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
    EXPECT_EQ(tMath::factorial(-1), 1);
    EXPECT_EQ(tMath::factorial(0), 1);
}

TEST(degrees, normal)
{
    constexpr float result = tMath::to_degrees(tMath::PI<float>);
    EXPECT_TRUE(tMath::approximately(result, 180.0f));
}

TEST(exp, normal)
{
    EXPECT_EQ(tMath::exp(5.0), std::exp(5.0));
    EXPECT_EQ(tMath::exp(5.0f), std::exp(5.0f));
}

TEST(min, normal)
{
    EXPECT_EQ(tMath::min(1, 2), std::min(1, 2));
}

TEST(max, normal)
{
    EXPECT_EQ(tMath::max(1, 2), std::max(1, 2));
}

TEST(abs, normal)
{
    EXPECT_TRUE(tMath::approximately(tMath::abs(-1.111f), 1.111f));
    EXPECT_TRUE(tMath::abs(-1) == 1);
}

TEST(clamp, normal)
{
    constexpr float a = 10;
    constexpr float result = tMath::clamp(a, 0.0f, tMath::Epsilon<float>);
    EXPECT_TRUE(result == tMath::Epsilon<float>);
}

TEST(lerp, normal)
{
    constexpr float result = tMath::lerp(-10.0f, 10.0f, 0.6);
    EXPECT_TRUE(tMath::approximately(result, 2.0f));
}

TEST(lerp_saturated, normal)
{
    constexpr float r1 = tMath::lerp_saturated(-10.0f, 10.0f, 1.2f);
    EXPECT_TRUE(tMath::approximately(r1, 10.0f));

    constexpr float r2 = tMath::lerp_saturated(-10.0f, 10.0f, -2.f);
    EXPECT_TRUE(tMath::approximately(r2, -10.0f));
}

TEST(min_max, normal)
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

TEST(triangle_functions, normal)
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

TEST(zero_divide, normal)
{
    // zero divide
    {
        int t = 0;
        float f = 10.0f / t;
        EXPECT_TRUE(tMath::is_infinity(f));
    }
    {
        int t = 0;
        float f = 0.0f / t;
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
