#include <tMath/vector2.hpp>
#include <tMath/vector3.hpp>
#include <tMath/vector4.hpp>

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

TEST(vector3, equal)
{
    {
        constexpr Vector3f v1 = {1.5, 2.5, 5.5};
        constexpr Vector3f v2 = { 1.5, 2.5, 5.5 };
        constexpr bool result = (v1 == v2);
        EXPECT_TRUE(result);
    }
    {
        Vector3f v1 = {1.5, 2.50001, 1};
        Vector3f v2 = { 1.5, 2.5, 1 };
        EXPECT_TRUE(!(v1 == v2));
    }
}

TEST(vector3, not_equal)
{
    constexpr Vector3f v1 = {1.5, 2.5, 5.51};
    constexpr Vector3f v2 = { 1.5, 2.5, 5.5 };
    constexpr bool result = (v1 != v2);
    EXPECT_TRUE(result);
}

TEST(vector3, self_add)
{
    Vector3f v1 = { 1.5, 2, 5.5 };
    constexpr Vector3f v2 = { 3.5, 2.5, 5.5 };
    v1 += v2;
    EXPECT_TRUE(tMath::approximately(v1, { 5, 4.5, 11.0 }));
}

TEST(vector3, self_sub)
{
    // -=
    Vector3f v1 = { 1.5, 2, 5.5 };
    constexpr Vector3f v2 = { 3.5, 2.5, 15.2 };
    v1 -= v2;
    EXPECT_TRUE(tMath::approximately(v1, { -2, -0.5, -9.7 }));
}

TEST(vector3, self_mul)
{
    // *=
    Vector3f v1 = { 1.5, 2, 5.5 };
    const float f = 3;
    v1 *= f;
    EXPECT_TRUE(tMath::approximately(v1, { 4.5, 6, 16.5 }));
}

TEST(vector3, self_div)
{
    // /=
    Vector3f v1 = { 1.5, 2, 5.5 };
    const float f = 3;
    v1 /= f;
    EXPECT_TRUE(tMath::approximately(v1, { 0.5, 2.0/3.0, 5.5/3.0 }));
}

TEST(vector3, add)
{
    // +
    constexpr Vector3f v1 = { 1.5, 2, 5.5 };
    constexpr Vector3f v2 = { 3.5, 2.5, 5.5 };
    constexpr Vector3f result = v1 + v2;
    EXPECT_TRUE(tMath::approximately(result, { 5, 4.5, 11.0 }));
}

TEST(vector3, sub)
{
    // -
    constexpr Vector3f v1 = { 1.5, 2, 15.5 };
    constexpr Vector3f v2 = { 3.5, 2.5, 5.2 };
    constexpr Vector3f result = v1 - v2;
    EXPECT_TRUE(tMath::approximately(result, { -2, -0.5, 10.3 }));
}

TEST(vector3, mul)
{
    // *
    constexpr Vector3f v1 = { 1.5, 2, 5.5 };
    constexpr float f = 3;
    constexpr Vector3f result = v1 * f;
    EXPECT_TRUE(tMath::approximately(result, { 4.5, 6, 16.5 }));
}

TEST(vector3, div)
{
    // /
    constexpr Vector3f v1 = { 1.5, 2, 5.5 };
    constexpr float f = 3;
    constexpr Vector3f result = v1 / f;
    EXPECT_TRUE(tMath::approximately(result, { 0.5, 2.0/3.0, 5.5/3.0 }));
}

TEST(vector3, dot)
{
    // dot
    {
        constexpr Vector3f v1 = { 1, 2, 3 };
        constexpr Vector3f v2 = { 2, 3, 4 };
        constexpr float f = tMath::dot(v1.data[0], v1.data[1], v1.data[2], v2.data[0], v2.data[1], v2.data[2]);
        EXPECT_TRUE(tMath::approximately(f, 20.0f));
    }
    {
        Vector3f v1 = { 1, 2, 3 };
        Vector3f v2 = { 2, 3, 4 };
        float f = tMath::dot(v1, v2);
        EXPECT_TRUE(tMath::approximately(f, 20.0f));
    }
}

TEST(vector3, hadamard_mul)
{
    constexpr Vector3f a = { 1, 2, 3 };
    constexpr Vector3f b = { 4, 5, 6 };
    constexpr Vector3f test = { 4, 10, 18 };
    constexpr Vector3f result = tMath::hadamard_mul(a, b);
    EXPECT_EQ(test, result);
}

TEST(vector3, hadamard_div)
{
    constexpr Vector3f a = { 1, 2, 3 };
    constexpr Vector3f b = { 4, 5, 6 };
    constexpr Vector3f test = { 1/4.0, 2.0/5, 0.5 };
    constexpr Vector3f result = tMath::hadamard_div(a, b);
    EXPECT_TRUE(tMath::approximately(test, result));
}

TEST(vector3, cross)
{
    // cross
    constexpr Vector3f v1 = { -1, 1, 6 };
    constexpr Vector3f v2 = { 1, 1, -10 };
    constexpr Vector3f result = tMath::cross(v1, v2);
    EXPECT_TRUE(tMath::approximately(result, { -16, -4, -2 }));
}

TEST(vector3, casts)
{
    // casts
    {
        constexpr Vector3f v3f = { 1.1, 2.2, 3.3 };
        constexpr Vector3i16 test = { 1, 2, 3 };
        constexpr Vector3i16 result = tMath::vector_cast<Vector3i16>(v3f);
        EXPECT_TRUE(result == test);
    }
    {
        const float x = 1.111111f;
        const float y = 2.222222f;
        const float z = 3.333333f;
        Vector3f v3f = { x, y, z };
        Vector3d test = { static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
        EXPECT_TRUE(tMath::approximately(tMath::vector_cast<Vector3d>(v3f), test));
        EXPECT_TRUE(tMath::vector_cast<Vector3d>(v3f) == test);
    }
}

TEST(vector3, approximately)
{
    Vector3f v3{};
    EXPECT_TRUE(tMath::approximately(v3, {0, 0, 0}));
}

TEST(vector3, to_degrees)
{
    // to degrees
    constexpr Vector3f degrees = { 180, 180, 360 };
    constexpr Vector3f result = tMath::to_radians(degrees);
    EXPECT_TRUE(tMath::approximately(result, { tMath::PI<float>, tMath::PI<float>, tMath::PI<float> * 2 }));
}

TEST(vector3, to_radians)
{
    constexpr Vector3f radians = { 2 * tMath::PI<float>, tMath::PI<float>, tMath::PI<float> };
    constexpr Vector3f result = tMath::to_degrees(radians);
    EXPECT_TRUE(tMath::approximately(result, { 360, 180, 180 }));
}

TEST(vector3, abs)
{
    Vector3f negative = { -180, -3, -150.123f };
    EXPECT_TRUE(tMath::approximately(tMath::abs(negative), { 180, 3, 150.123f }));
}

TEST(vector3, magnitude)
{
    {
        Vector3f vec = { -1, 1, -1 };
        auto f = tMath::magnitude(vec);
        EXPECT_TRUE(tMath::approximately(f, 1.732050f));
    }
    {
        // Vector3f vec = { -1, 1, -1 };
        auto f = tMath::magnitude(-1.0f, 1.0f, -1.0f);
        EXPECT_TRUE(tMath::approximately(f, 1.732050f));
    }
}

TEST(vector3, normalized)
{
    Vector3f vec = { 1000, 0, 1000 };
    EXPECT_TRUE(tMath::approximately(tMath::normalized(vec), { 0.707106f, 0, 0.707106f }));
}

TEST(vector3, normalize_inplace)
{
    Vector3f vec = { 1000, 0, 1000 };
    tMath::normalize_inplace(vec);
    EXPECT_TRUE(tMath::approximately(vec, { 0.707106f, 0, 0.707106f }));
}

TEST(vector3, sint_normalized)
{
    // sint normalized
    Vector3i32 vi = { 1234, 1234, 1234 };
    EXPECT_TRUE(tMath::approximately(tMath::normalized<Vector3f>(vi), { 0.577350f, 0.577350f, 0.577350f }));
}

TEST(vector3, distance)
{
    // distance
    {
        // f32
        Vector3f v1 = { 5, 10, 5 };
        Vector3f v2 = { 2, -5, 5 };
        auto dis = tMath::distance(v1, v2);
        EXPECT_TRUE(tMath::approximately(dis, 15.297058f));
    }
    {
        // i32
        Vector3i32 v1 = { 5, 10, 9 };
        Vector3i32 v2 = { 2, -5, 9 };
        auto dis = tMath::distance(v1, v2); // int32 返回的是 double
        EXPECT_TRUE(tMath::approximately(dis, 15.29705854077835449));
    }
    {
        // i16
        Vector3i16 v1 = { 5, 10, 90 };
        Vector3i16 v2 = { 2, -5, 90 };
        auto dis = tMath::distance(v1, v2); // int16 返回的是 float
        EXPECT_TRUE(tMath::approximately(dis, 15.297058f));
    }
}

TEST(vector3, lerp)
{
    // lerp
    constexpr Vector3f a = { -10, 10, -10 };
    constexpr Vector3f b = { 10, 30, 10 };
    constexpr Vector3f result = tMath::lerp(a, b, 0.6f);
    EXPECT_TRUE(tMath::approximately(result, { 2.0f, 22.0f, 2.0f }));
}

TEST(vector3, zero_div)
{
    // zero divide
    {
        Vector3f zero = {0, 0};
        Vector3f n = tMath::safe_normalized(zero, {5, 5, 1});
        EXPECT_TRUE(tMath::approximately(n, {5, 5, 1}));
    }
    {
        Vector3i32 zero = {0, 0, 0};
        Vector3f n = tMath::safe_normalized<Vector3f>(zero, {10, 5, 6});
        EXPECT_TRUE(tMath::approximately(n, {10, 5, 6}));
    }
    {
        Vector3f zero = {0, 0, 0};
        tMath::safe_normalize_inplace(zero, {5, 10, -5});
        EXPECT_TRUE(tMath::approximately(zero, {5, 10, -5}));
    }
    {
        // safe divide
        Vector3f v = {10, 10, 10};
        float zero = 0;
        EXPECT_TRUE(tMath::approximately(tMath::safe_divide(v, zero, {100, 100, 100}), {100, 100, 100}));
    }
    {
        Vector3f v = {10, 10, 10};
        float zero = 0;
        tMath::safe_divide_inplace(v, zero, {100, 100, 98});
        EXPECT_TRUE(tMath::approximately(v, {100, 100, 98}));
    }
}



