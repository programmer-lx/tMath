#include <tMath/vector.hpp>

#include "../test.hpp"

TMATH_DIAGNOSTICS_PUSH

#if defined(TMATH_COMPILER_CLANG)
TMATH_IGNORE_WARNING("-Wmissing-braces")
#endif

struct Vector4f
{
    TMATH_FULL_VECTOR4(Vector4f, float)
};

struct Vector4d
{
    TMATH_FULL_VECTOR4(Vector4d, double)
};

struct Vector4i16
{
    TMATH_FULL_VECTOR4(Vector4i16, int16_t)
};

struct Vector4i32
{
    TMATH_FULL_VECTOR4(Vector4i32, int32_t)
};

TEST(vector4, constexpr_test)
{
    constexpr Vector4f a = { 1, 1, 1, 1 };
    constexpr Vector4f b = { 1, 1, 1, 1 };
    {
        // ==
        constexpr bool result = (a == b);
        EXPECT_TRUE(result == true);
    }
    {
        // +
        constexpr Vector4f t = { 0, 0, 0, 0 };
        constexpr Vector4f result = t + a;
        EXPECT_TRUE(result == a);
    }
    {
        // -
        constexpr Vector4f t = { 0, 0, 0, 0 };
        constexpr Vector4f result = a - t;
        EXPECT_TRUE(result == a);
    }
    {
        // *
        constexpr Vector4f result = a * 1.0f;
        EXPECT_TRUE(result == a);
    }
    {
        // /
        constexpr Vector4f result = a / 1.0f;
        EXPECT_TRUE(result == a);
    }
    {
        // cast
        constexpr Vector4i32 result = tmath::vector_cast<Vector4i32>(a);
        constexpr Vector4i32 test = { 1, 1, 1, 1 };
        EXPECT_TRUE(result == test);
    }
    {
        constexpr Vector4f degrees = tmath::to_degrees(Vector4f{ tmath::PI<float>, tmath::PI<float>, tmath::PI<float>, tmath::PI<float> });
        constexpr Vector4f test = { 180, 180, 180, 180 };
        EXPECT_TRUE(degrees == test);
    }
    {
        constexpr Vector4f radians = tmath::to_radians(Vector4f{ 180, 180, 180, 180 });
        constexpr Vector4f test = { tmath::PI<float>, tmath::PI<float>, tmath::PI<float>, tmath::PI<float> };
        EXPECT_TRUE(radians == test);
    }
    {
        // lerp
         constexpr Vector4f result = tmath::lerp(a, b, 0.5f);
        EXPECT_TRUE(result == a);
    }
}

TEST(vector4, fields)
{
    {
        constexpr Vector4f v{ 1, 2, 3, 4 };
        EXPECT_TRUE(v[0] == 1);
        EXPECT_TRUE(v[1] == 2);
        EXPECT_TRUE(v[2] == 3);
        EXPECT_TRUE(v[3] == 4);

        EXPECT_TRUE(v.x == 1);
        EXPECT_TRUE(v.y == 2);
        EXPECT_TRUE(v.z == 3);
        EXPECT_TRUE(v.w == 4);

        EXPECT_TRUE(v.r == 1);
        EXPECT_TRUE(v.g == 2);
        EXPECT_TRUE(v.b == 3);
        EXPECT_TRUE(v.a == 4);

        EXPECT_TRUE(v.data[0] == 1);
        EXPECT_TRUE(v.data[1] == 2);
        EXPECT_TRUE(v.data[2] == 3);
        EXPECT_TRUE(v.data[3] == 4);
    }
    {
        Vector4f v = { 1, 2, 3, 4 };
        v.r = 5;
        v.g = 6;
        v.b = 7;
        v.a = 8;

        Vector4f v2 = { 1, 2, 3, 4 };
        float result = tmath::dot(v, v2);
        EXPECT_TRUE(tmath::approximately(result, static_cast<float>(1 * 5 + 2 * 6 + 3 * 7 + 4 * 8)));
    }
    {
        Vector4f v = {
            .r = 1,
            .g = 2,
            .b = 3,
            .a = 4
        };

        Vector4f v2 = { 1, 2, 3, 4 };
        EXPECT_TRUE(tmath::approximately_all(v, v2));
        EXPECT_TRUE(v == v2);
    }
}


TEST(vector4, quat_tag)
{
    // quat tag
    EXPECT_TRUE(tmath::is_vector4_float<Vector4f>);
    EXPECT_TRUE(!tmath::is_quat<Vector4f>);
}

TEST(vector4, dot)
{
    // dot
    {
        constexpr Vector4f v1 = { 1, 2, 3, 3 };
        constexpr Vector4f v2 = { 2, 3, 4, 4 };
        constexpr float f = tmath::dot(v1.data[0], v1.data[1], v1.data[2], v1.data[3], v2.data[0], v2.data[1], v2.data[2], v2.data[3]);
        EXPECT_TRUE(tmath::approximately(f, 32.0f));
    }
    {
        Vector4f v1 = { 1, 2, 3, 3 };
        Vector4f v2 = { 2, 3, 4, 4 };
        float f = tmath::dot(v1, v2);
        EXPECT_TRUE(tmath::approximately(f, 32.0f));
    }
}

TEST(vector4, cross)
{
    // cross
    constexpr Vector4f v1 = { -1, 1, 6, 5 };
    constexpr Vector4f v2 = { 1, 1, -10, 10 };
    constexpr Vector4f result = tmath::cross(v1, v2);
    EXPECT_TRUE(tmath::approximately_all(result, { -16, -4, -2, 0 }));
}

TMATH_DIAGNOSTICS_POP
