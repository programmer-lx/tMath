#include <tMath/vector.hpp>

#include "test.hpp"

struct Vector2f
{
    TMATH_FULL_VECTOR2(Vector2f, float)
};

struct Vector2d
{
    TMATH_FULL_VECTOR2(Vector2d, double)
};

struct Vector2i16
{
    TMATH_FULL_VECTOR2(Vector2i16, int16_t)
};

struct Vector2i32
{
    TMATH_FULL_VECTOR2(Vector2i32, int32_t)
};


TEST(vector2, fields)
{
    Vector2f v{1, 2};
    EXPECT_TRUE(v[0] == 1);
    EXPECT_TRUE(v[1] == 2);
    EXPECT_TRUE(v.x == 1);
    EXPECT_TRUE(v.y == 2);
    EXPECT_TRUE(v.r == 1);
    EXPECT_TRUE(v.g == 2);
    EXPECT_TRUE(v.u == 1);
    EXPECT_TRUE(v.v == 2);
    EXPECT_TRUE(v.data[0] == 1);
    EXPECT_TRUE(v.data[1] == 2);
}

TEST(vector2, dot)
{
    {
        constexpr Vector2f v1 = { 1, 2 };
        constexpr Vector2f v2 = { 2, 3 };
        constexpr float f = tMath::dot(v1.data[0], v1.data[1], v2.data[0], v2.data[1]);
        EXPECT_TRUE(tMath::approximately(f, 8.0f));
    }
    {
        Vector2f v1 = { 1, 2 };
        Vector2f v2 = { 2, 3 };
        float f = tMath::dot(v1, v2);
        EXPECT_TRUE(tMath::approximately(f, 8.0f));
    }
}

TEST(vector2, cross)
{
    {
        // v1 -> v2 是顺时针
        constexpr Vector2f v1 = { -1, 1 };
        constexpr Vector2f v2 = { 1, 1 };
        constexpr float result = tMath::cross(v1, v2);
        EXPECT_TRUE(result < 0);
    }
    {
        // v1 -> v2 是逆时针
        Vector2f v1 = { -1, 1 };
        Vector2f v2 = { 1, 1 };
        EXPECT_TRUE(tMath::cross(v2, v1) > 0);
    }
    {
        // v1, v2平行
        Vector2f v1 = { -1, -1 };
        Vector2f v2 = { 1, 1 };
        EXPECT_TRUE(tMath::cross(v1, v2) == 0);
    }

    {
        // v1 -> v2 是顺时针
        Vector2f v1 = { -1, 1 };
        Vector2f v2 = { 1, 1 };
        EXPECT_TRUE(tMath::cross(v1.x, v1.y, v2.x, v2.y) < 0);
    }
    {
        // v1 -> v2 是逆时针
        Vector2f v1 = { -1, 1 };
        Vector2f v2 = { 1, 1 };
        EXPECT_TRUE(tMath::cross(v2.x, v2.y, v1.x, v1.y) > 0);
    }
    {
        // v1, v2平行
        Vector2f v1 = { -1, -1 };
        Vector2f v2 = { 1, 1 };
        EXPECT_TRUE(tMath::cross(v1.x, v1.y, v2.x, v2.y) == 0);
    }
}