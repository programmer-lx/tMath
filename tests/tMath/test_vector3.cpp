#include <tMath/vector.hpp>

#include "../test.hpp"

struct Vector3f
{
    TMATH_FULL_VECTOR3(Vector3f, float)
};

struct Vector3d
{
    TMATH_FULL_VECTOR3(Vector3d, double)
};

struct Vector3i32
{
    TMATH_FULL_VECTOR3(Vector3i32, int32_t)
};

struct Vector3i16
{
    TMATH_FULL_VECTOR3(Vector3i16, int16_t)
};


TEST(vector3, fields)
{
    Vector3f v{1, 2, 3};
    EXPECT_TRUE(v[0] == 1);
    EXPECT_TRUE(v[1] == 2);
    EXPECT_TRUE(v[2] == 3);
    EXPECT_TRUE(v.x == 1);
    EXPECT_TRUE(v.y == 2);
    EXPECT_TRUE(v.z == 3);
    EXPECT_TRUE(v.r == 1);
    EXPECT_TRUE(v.g == 2);
    EXPECT_TRUE(v.b == 3);
    EXPECT_TRUE(v.data[0] == 1);
    EXPECT_TRUE(v.data[1] == 2);
    EXPECT_TRUE(v.data[2] == 3);
}

TEST(vector3, dot)
{
    // dot
    {
        constexpr Vector3f v1 = { 1, 2, 3 };
        constexpr Vector3f v2 = { 2, 3, 4 };
        constexpr float f = tmath::dot(v1.data[0], v1.data[1], v1.data[2], v2.data[0], v2.data[1], v2.data[2]);
        EXPECT_TRUE(tmath::approximately(f, 20.0f));
    }
    {
        Vector3f v1 = { 1, 2, 3 };
        Vector3f v2 = { 2, 3, 4 };
        float f = tmath::dot(v1, v2);
        EXPECT_TRUE(tmath::approximately(f, 20.0f));
    }
}

TEST(vector3, cross)
{
    // cross
    constexpr Vector3f v1 = { -1, 1, 6 };
    constexpr Vector3f v2 = { 1, 1, -10 };
    constexpr Vector3f result = tmath::cross(v1, v2);
    EXPECT_TRUE(tmath::approximately_all(result, { -16, -4, -2 }));
}
