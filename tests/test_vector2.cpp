#include <tMath/vector2.hpp>
#include <tMath/vector3.hpp>
#include <tMath/vector4.hpp>

#include "test.hpp"

struct Vector2f
{
    TMATH_VECTOR2(Vector2f, float)
};

struct Vector2d
{
    TMATH_VECTOR2(Vector2d, double)
};

struct Vector2i16
{
    TMATH_VECTOR2(Vector2i16, int16_t)
};

struct Vector2i32
{
    TMATH_VECTOR2(Vector2i32, int32_t)
};

struct Vector3f
{
    TMATH_VECTOR3(Vector3f, float)
};

struct Vector3d
{
    TMATH_VECTOR3(Vector3d, double)
};

struct Vector3i32
{
    TMATH_VECTOR3(Vector3i32, int32_t)
};

struct Vector3i16
{
    TMATH_VECTOR3(Vector3i16, int16_t)
};

struct Vector4f
{
    TMATH_VECTOR4(Vector4f, float)
};

struct Vector4d
{
    TMATH_VECTOR4(Vector4d, double)
};

struct Vector4i16
{
    TMATH_VECTOR4(Vector4i16, int16_t)
};

struct Vector4i32
{
    TMATH_VECTOR4(Vector4i32, int32_t)
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
    EXPECT_TRUE(v.data[0] == 1);
    EXPECT_TRUE(v.data[1] == 2);
}

TEST(vector2, equal)
{
    {
        Vector2f v1 = {1.5, 2.5};
        Vector2f v2 = { 1.5, 2.5 };
        EXPECT_TRUE(v1 == v2);
    }
    {
        Vector2f v1 = {1.5, 2.50001};
        Vector2f v2 = { 1.5, 2.5 };
        EXPECT_TRUE(!(v1 == v2));
    }
}

TEST(vector2, no_equal)
{
    {
        Vector2f v1 = {1.5, 2.5};
        Vector2f v2 = { 1.58, 2.5 };
        EXPECT_TRUE(v1 != v2);
    }
    {
        Vector2f v1 = {1.5, 2.5};
        Vector2f v2 = { 1.5, 2.51 };
        EXPECT_FALSE(tMath::approximately(v1, v2));
    }
}

TEST(vector2, self_add)
{
    Vector2f v1 = { 1.5, 2 };
    constexpr Vector2f v2 = { 3.5, 2.5 };
    v1 += v2;
    EXPECT_TRUE(tMath::approximately(v1, { 5, 4.5 }));
}

TEST(vector2, self_sub)
{
    Vector2f v1 = { 1.5, 2 };
    constexpr Vector2f v2 = { 3.5, 2.5 };
    v1 -= v2;
    EXPECT_TRUE(tMath::approximately(v1, { -2, -0.5 }));
}

TEST(vector2, self_mul)
{
    Vector2f v1 = { 1.5, 2 };
    const float f = 3;
    v1 *= f;
    EXPECT_TRUE(tMath::approximately(v1, { 4.5, 6 }));
}

TEST(vector2, self_div)
{
    // /=
    Vector2f v1 = { 1.5, 2 };
    const float f = 3;
    v1 /= f;
    EXPECT_TRUE(tMath::approximately(v1, { 0.5, 2.0/3.0 }));
}

TEST(vector2, safe_self_div)
{
    // safe /=
    Vector2f v1 = {1.5, 2};
    const float zero = 0;
    tMath::safe_divide_inplace(v1, zero, {5, 6});
    EXPECT_TRUE(tMath::approximately({5, 6}, v1));
}

TEST(vector2, add)
{
    // +
    Vector2f v1 = { 1.5, 2 };
    Vector2f v2 = { 3.5, 2.5 };
    EXPECT_TRUE(tMath::approximately(v1 + v2, { 5, 4.5 }));
}

TEST(vector2, sub)
{
    // -
    Vector2f v1 = { 1.5, 2 };
    constexpr Vector2f v2 = { 3.5, 2.5 };
    EXPECT_TRUE(tMath::approximately(v1 - v2, { -2, -0.5 }));
}

TEST(vector2, mul)
{
    // *
    Vector2f v1 = { 1.5, 2 };
    const float f = 3;
    EXPECT_TRUE(tMath::approximately(v1 * f, { 4.5, 6 }));
}

TEST(vector2, div)
{
    // /
    Vector2f v1 = { 1.5, 2 };
    const float f = 3;
    EXPECT_TRUE(tMath::approximately(v1 / f, { 0.5, 2.0/3.0 }));
}

TEST(vector2, safe_div)
{
    // safe /
    Vector2f v = {1.5, 2};
    const float zero = 0;
    EXPECT_TRUE(tMath::approximately(tMath::safe_divide(v, zero, {5, 6}), {5, 6}));
}

TEST(vector2, dot)
{
    {
        Vector2f v1 = { 1, 2 };
        Vector2f v2 = { 2, 3 };
        float f = tMath::dot(v1.x, v1.y, v2.x, v2.y);
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
        Vector2f v1 = { -1, 1 };
        Vector2f v2 = { 1, 1 };
        EXPECT_TRUE(tMath::cross(v1, v2) < 0);
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

TEST(vector2, cast)
{
    {
        Vector2f v2f = { 1.1, 2.2 };
        Vector2i16 test = { 1, 2 };
        EXPECT_TRUE(tMath::vector_cast<Vector2i16>(v2f) == test);
    }
    {
        const float x = 1.111111f;
        const float y = 2.222222f;
        Vector2f v2f = { x, y };
        Vector2d test = { static_cast<double>(x), static_cast<double>(y) };
        EXPECT_TRUE(tMath::approximately(tMath::vector_cast<Vector2d>(v2f), test));
        EXPECT_TRUE(tMath::vector_cast<Vector2d>(v2f) == test);
    }
}

TEST(vector2, approximately)
{
    {
        Vector2f v2{};
        EXPECT_TRUE(tMath::approximately(v2, {0, 0}));
    }
}

TEST(vector2, to_radians)
{
    Vector2f degrees = { 180, 180 };
    EXPECT_TRUE(tMath::approximately(tMath::to_radians(degrees), { tMath::PI<float>, tMath::PI<float> }));
}

TEST(vector2, to_degrees)
{
    Vector2f radians = { 2 * tMath::PI<float>, tMath::PI<float> };
    EXPECT_TRUE(tMath::approximately(tMath::to_degrees(radians), { 360, 180 }));
}

TEST(vector2, abs)
{
    Vector2f negative = { -180, -3 };
    EXPECT_TRUE(tMath::approximately(tMath::abs(negative), { 180, 3 }));
}

TEST(vector2, magnitude)
{
    {
        Vector2f vec = { -1, 1 };
        auto f = tMath::magnitude(vec);
        EXPECT_TRUE(tMath::approximately(f, 1.414213f));
    }
    {
        // Vector2f vec = { -1, 1 };
        auto f = tMath::magnitude(-1.0f, 1.0f);
        EXPECT_TRUE(tMath::approximately(f, 1.414213f));
    }
}

TEST(vector2, normalized)
{
    Vector2f vec = { 1000, 0 };
    EXPECT_TRUE(tMath::approximately(tMath::normalized(vec), { 1, 0 }));
}

TEST(vector2, normalize_inplace)
{
    Vector2f vec = {1000, 0};
    tMath::normalize_inplace(vec);
    EXPECT_TRUE(tMath::approximately(vec, { 1, 0 }));
}

TEST(vector2, sint_normalized)
{
    // sint normalized
    Vector2i32 vi = { 1234, 1234 };
    EXPECT_TRUE(tMath::approximately(tMath::normalized<Vector2f>(vi), { 0.707106f, 0.707106f }));
}

TEST(vector2, f32_distance)
{
    // f32
    Vector2f v1 = { 5, 10 };
    Vector2f v2 = { 2, -5 };
    auto dis = tMath::distance(v1, v2);
    EXPECT_TRUE(tMath::approximately(dis, 15.297058f));
}

TEST(vector2, i32_distance)
{
    // i32
    Vector2i32 v1 = { 5, 10 };
    Vector2i32 v2 = { 2, -5 };
    auto dis = tMath::distance(v1, v2); // int32 返回的是 double
    EXPECT_TRUE(tMath::approximately(dis, 15.29705854077835449));
}

TEST(vector2, i16_distance)
{
    // i16
    Vector2i16 v1 = { 5, 10 };
    Vector2i16 v2 = { 2, -5 };
    auto dis = tMath::distance(v1, v2); // int16 返回的是 float
    EXPECT_TRUE(tMath::approximately(dis, 15.297058f));
}

TEST(vector2, lerp)
{
    // lerp
    Vector2f a = { -10, 10 };
    Vector2f b = { 10, 30 };
    EXPECT_TRUE(tMath::approximately(tMath::lerp(a, b, 0.6f), { 2.0f, 22.0f }));
}

TEST(vector2, zero_div)
{
    {
        Vector2f zero = {0, 0};
        Vector2f n = tMath::safe_normalized(zero, {5, 5});
        EXPECT_TRUE(tMath::approximately(n, {5, 5}));
    }
    {
        Vector2i32 zero = {0, 0};
        Vector2f n = tMath::safe_normalized<Vector2f>(zero, {10, 5});
        EXPECT_TRUE(tMath::approximately(n, {10, 5}));
    }
    {
        Vector2f zero = {0, 0};
        tMath::safe_normalize_inplace(zero, {5, 10});
        EXPECT_TRUE(tMath::approximately(zero, {5, 10}));
    }
    {
        // safe divide
        Vector2f v = {10, 10};
        float zero = 0;
        EXPECT_TRUE(tMath::approximately(tMath::safe_divide(v, zero, {100, 100}), {100, 100}));
    }
    {
        Vector2f v = {10, 10};
        float zero = 0;
        tMath::safe_divide_inplace(v, zero, {100, 100});
        EXPECT_TRUE(tMath::approximately(v, {100, 100}));
    }
}

TMATH_TEST_MAIN()
