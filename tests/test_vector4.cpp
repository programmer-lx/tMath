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
        constexpr Vector4i32 result = tMath::vector_cast<Vector4i32>(a);
        constexpr Vector4i32 test = { 1, 1, 1, 1 };
        EXPECT_TRUE(result == test);
    }
    {
        constexpr Vector4f degrees = tMath::to_degrees(Vector4f{ tMath::PI<float>, tMath::PI<float>, tMath::PI<float>, tMath::PI<float> });
        constexpr Vector4f test = { 180, 180, 180, 180 };
        EXPECT_TRUE(degrees == test);
    }
    {
        constexpr Vector4f radians = tMath::to_radians(Vector4f{ 180, 180, 180, 180 });
        constexpr Vector4f test = { tMath::PI<float>, tMath::PI<float>, tMath::PI<float>, tMath::PI<float> };
        EXPECT_TRUE(radians == test);
    }
    {
        // lerp
         constexpr Vector4f result = tMath::lerp(a, b, 0.5f);
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
        float result = tMath::dot(v, v2);
        EXPECT_TRUE(tMath::approximately(result, static_cast<float>(1 * 5 + 2 * 6 + 3 * 7 + 4 * 8)));
    }
    {
        Vector4f v = {
            .r = 1,
            .g = 2,
            .b = 3,
            .a = 4
        };

        Vector4f v2 = { 1, 2, 3, 4 };
        EXPECT_TRUE(tMath::approximately(v, v2));
        EXPECT_TRUE(v == v2);
    }
}


TEST(vector4, quat_tag)
{
    // quat tag
    EXPECT_TRUE(tMath::is_vector4_float<Vector4f>);
    EXPECT_TRUE(!tMath::is_quat<Vector4f>);
}

TEST(vector4, equal)
{
    // ==
    {
        Vector4f v1 = { 1.5, 2.5, 5.5, 6.6 };
        Vector4f v2 = { 1.5, 2.5, 5.5, 6.6 };
        EXPECT_TRUE(v1 == v2);
    }
    {
        Vector4f v1 = {1, 2, 1, 1.1};
        Vector4f v2 = { 1, 2, 1, 1.2 };
        EXPECT_TRUE(!(v1 == v2));
    }
}

TEST(vector4, nor_equal)
{
    // !=
    {
        Vector4f v1 = {1.5, 2.5, 5.5, 1.1};
        Vector4f v2 = { 1.5, 2.5, 5.5, 1.2 };
        EXPECT_TRUE(v1 != v2);
    }
}

TEST(vector4, self_add)
{
    // +=
    Vector4f v1 = { 1.5, 2, 5.5, 1 };
    Vector4f v2 = { 3.5, 2.5, 5.5, 5.678 };
    Vector4f v3 = { 1, 2, 3, 4 };
    v1 += v2 += v3;
    EXPECT_TRUE(tMath::approximately(v1, { 6, 6.5, 14.0, 10.678 }));
}

TEST(vector4, self_sub)
{
    // -=
    Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
    Vector4f v2 = { 3.5, 2.5, 15.2, 15.2 };
    Vector4f v3 = { 1, 2, 3, 4 };
    v1 -= v2 -= v3;
    EXPECT_TRUE(tMath::approximately(v1, { -1, 1.5, -6.7, -5.7 }));
}

TEST(vector4, self_mul)
{
    // *=
    Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
    float f = 3;
    v1 *= f *= 3;
    EXPECT_TRUE(tMath::approximately(v1, { 13.5, 18, 49.5, 49.5 }));
}

TEST(vector4, self_div)
{
    // /=
    Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
    float f = 3;
    v1 /= f /= 2;
    EXPECT_TRUE(tMath::approximately(v1, { 1, 2.0/1.5, 5.5/1.5, 5.5/1.5 }));
}

TEST(vector4, add)
{
    // +
    Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
    Vector4f v2 = { 3.5, 2.5, 5.5, 5.5 };
    EXPECT_TRUE(tMath::approximately(v1 + v2, { 5, 4.5, 11.0, 11.0 }));
}

TEST(vector4, sub)
{
    // -
    Vector4f v1 = { 1.5, 2, 15.5, 15.5 };
    constexpr Vector4f v2 = { 3.5, 2.5, 5.2, 5.2 };
    EXPECT_TRUE(tMath::approximately(v1 - v2, { -2, -0.5, 10.3, 10.3 }));
}

TEST(vector4, mul)
{
    // *
    Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
    const float f = 3;
    EXPECT_TRUE(tMath::approximately(v1 * f, { 4.5, 6, 16.5, 16.5 }));
}

TEST(vector4, div)
{
    // /
    Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
    const float f = 3;
    EXPECT_TRUE(tMath::approximately(v1 / f, { 0.5, 2.0/3.0, 5.5/3.0, 5.5/3.0 }));
}

TEST(vector4, hadamard_mul)
{
    constexpr Vector4f a = {1, 2, 3, 4};
    constexpr Vector4f b = {4, 5, 6, 7};
    constexpr Vector4f test = {4, 10, 18, 28};
    constexpr Vector4f result = tMath::hadamard_mul(a, b);
    EXPECT_EQ(test, result);
}

TEST(vector4, hadamard_div)
{
    constexpr Vector4f a = {1, 2, 3, 4};
    constexpr Vector4f b = {4, 5, 6, 7};
    constexpr Vector4f test = {1.0/4, 2.0/5, 0.5, 4.0/7};
    constexpr Vector4f result = tMath::hadamard_div(a, b);
    EXPECT_TRUE(tMath::approximately(test, result));
}

TEST(vector4, dot)
{
    // dot
    {
        constexpr Vector4f v1 = { 1, 2, 3, 3 };
        constexpr Vector4f v2 = { 2, 3, 4, 4 };
        constexpr float f = tMath::dot(v1.data[0], v1.data[1], v1.data[2], v1.data[3], v2.data[0], v2.data[1], v2.data[2], v2.data[3]);
        EXPECT_TRUE(tMath::approximately(f, 32.0f));
    }
    {
        Vector4f v1 = { 1, 2, 3, 3 };
        Vector4f v2 = { 2, 3, 4, 4 };
        float f = tMath::dot(v1, v2);
        EXPECT_TRUE(tMath::approximately(f, 32.0f));
    }
}

TEST(vector4, cross)
{
    // cross
    constexpr Vector4f v1 = { -1, 1, 6, 5 };
    constexpr Vector4f v2 = { 1, 1, -10, 10 };
    constexpr Vector4f result = tMath::cross(v1, v2);
    EXPECT_TRUE(tMath::approximately(result, { -16, -4, -2, 0 }));
}

TEST(vector4, casts)
{
    // casts
    {
        constexpr Vector4f v4f = { 1.1, 2.2, 3.3, 4.4 };
        constexpr Vector4i16 test = { 1, 2, 3, 4 };
        constexpr Vector4i16 result = tMath::vector_cast<Vector4i16>(v4f);
        EXPECT_TRUE(result == test);
    }
    {
        const float x = 1.111111f;
        const float y = 2.222222f;
        const float z = 3.333333f;
        const float w = 4.444444f;
        Vector4f v4f = { x, y, z, w };
        Vector4d test = { static_cast<double>(x), static_cast<double>(y), static_cast<double>(z), static_cast<double>(w) };
        EXPECT_TRUE(tMath::approximately(tMath::vector_cast<Vector4d>(v4f), test));
        EXPECT_TRUE(tMath::vector_cast<Vector4d>(v4f) == test);
    }
}

TEST(vector4, approximately)
{
    Vector4f v4{};
    EXPECT_TRUE(tMath::approximately(v4, {0, 0, 0, 0}));
}

TEST(vector4, to_radians)
{
    // to radians
    Vector4f degrees = { 180, 180, 360, 90 };
    EXPECT_TRUE(tMath::approximately(tMath::to_radians(degrees), { tMath::PI<float>, tMath::PI<float>, tMath::PI<float> * 2, tMath::HalfPI<float> }));
}

TEST(vector4, to_degrees)
{
    Vector4f radians = { 2 * tMath::PI<float>, tMath::PI<float>, tMath::PI<float>, tMath::HalfPI<float> };
    EXPECT_TRUE(tMath::approximately(tMath::to_degrees(radians), { 360, 180, 180, 90 }));
}

TEST(vector4, abs)
{
    Vector4f negative = { -180, -3, -150.123f, -256.12 };
    EXPECT_TRUE(tMath::approximately(tMath::abs(negative), { 180, 3, 150.123f, 256.12 }));
}

TEST(vector4, triangle_functions)
{
    Vector4f v = { -180, -3, -150.123f, -256.12 };
    EXPECT_TRUE(tMath::approximately(tMath::sin(v), { std::sin(-180.0f), std::sin(-3.0f), std::sin(-150.123f), std::sin(-256.12f) }));
    EXPECT_TRUE(tMath::approximately(tMath::cos(v), { std::cos(-180.0f), std::cos(-3.0f), std::cos(-150.123f), std::cos(-256.12f) }));
    EXPECT_TRUE(tMath::approximately(tMath::tan(v), { std::tan(-180.0f), std::tan(-3.0f), std::tan(-150.123f), std::tan(-256.12f) }));
}

TEST(vector4, magnitude)
{
    {
        Vector4f vec = { -1, 1, -1, 2 };
        auto f = tMath::magnitude(vec);
        EXPECT_TRUE(tMath::approximately(f, 2.645751f));
    }
    {
        // Vector4f vec = { -1, 1, -1, 2 };
        auto f = tMath::magnitude(-1.0f, 1.0f, -1.0f, 2.0f);
        EXPECT_TRUE(tMath::approximately(f, 2.645751f));
    }
}

TEST(vector4, normalized)
{
    Vector4f vec = { 1000, 0, 1000, 1000 };
    EXPECT_TRUE(tMath::approximately(tMath::normalized(vec), { 0.577350f, 0, 0.577350f, 0.577350f }));
}

TEST(vector4, normalize_inplace)
{
    Vector4f vec = { 1000, 0, 1000, 1000 };
    tMath::normalize_inplace(vec);
    EXPECT_TRUE(tMath::approximately(vec, { 0.577350f, 0, 0.577350f, 0.577350f }));
}

TEST(vector4, sint_normalized)
{
    // sint normalized
    Vector4i32 vi = { 1234, 1234, 1234, 1234 };
    EXPECT_TRUE(tMath::approximately(tMath::normalized<Vector4f>(vi), { 0.5f, 0.5f, 0.5f, 0.5f }));
}

TEST(vector4, distance)
{
    // distance
    {
        // f32
        Vector4f v1 = { 5, 10, 5, 10 };
        Vector4f v2 = { 2, 10, 5, -5 };
        auto dis = tMath::distance(v1, v2);
        EXPECT_TRUE(tMath::approximately(dis, 15.297058f));
    }
    {
        // i32
        Vector4i32 v1 = { 5, 10, 9, 10 };
        Vector4i32 v2 = { 2, 10, 9, -5 };
        auto dis = tMath::distance(v1, v2); // int32 返回的是 double
        EXPECT_TRUE(tMath::approximately(dis, 15.29705854077835449));
    }
    {
        // i16
        Vector4i16 v1 = { 5, 10, 90 ,10 };
        Vector4i16 v2 = { 2, 10, 90, -5 };
        auto dis = tMath::distance(v1, v2); // int16 返回的是 float
        EXPECT_TRUE(tMath::approximately(dis, 15.297058f));
    }
}

TEST(vector4, lerp)
{
    // lerp
    Vector4f a = { -10, 10, -10, -10 };
    Vector4f b = { 10, 30, 10, 10 };
    EXPECT_TRUE(tMath::approximately(tMath::lerp(a, b, 0.6f), { 2.0f, 22.0f, 2.0f, 2.0f }));
}

TEST(vector4, zero_div)
{
    // zero divide
    {
        Vector4f zero = {0, 0, 0, 0};
        Vector4f n = tMath::safe_normalized(zero, {5, 5, 1, 6});
        EXPECT_TRUE(tMath::approximately(n, {5, 5, 1, 6}));
    }
    {
        Vector4i32 zero = {0, 0, 0, 0};
        Vector4f n = tMath::safe_normalized<Vector4f>(zero, {10, 5, 6, 10});
        EXPECT_TRUE(tMath::approximately(n, {10, 5, 6, 10}));
    }
    {
        Vector4f zero = {0, 0, 0 , 0};
        tMath::safe_normalize_inplace(zero, {5, 10, -5, -8});
        EXPECT_TRUE(tMath::approximately(zero, {5, 10, -5, -8}));
    }
    {
        // safe divide
        Vector4f v = {10, 10, 10, 10};
        float zero = 0;
        EXPECT_TRUE(tMath::approximately(tMath::safe_divide(v, zero, {100, 100, 100, 99}), {100, 100, 100, 99}));
    }
    {
        Vector4f v = {10, 10, 10, 5};
        float zero = 0;
        tMath::safe_divide_inplace(v, zero, {100, 100, 98, 97});
        EXPECT_TRUE(tMath::approximately(v, {100, 100, 98, 97}));
    }
}



