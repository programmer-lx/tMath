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


void test_vector2()
{
    TEST_TITLE("test vector2");

    {
        {
            Vector2f v{1, 2};
            TEST_BOOL(v[0] == 1);
            TEST_BOOL(v[1] == 2);
            TEST_BOOL(v.x == 1);
            TEST_BOOL(v.y == 2);
            TEST_BOOL(v.r == 1);
            TEST_BOOL(v.g == 2);
            TEST_BOOL(v.data[0] == 1);
            TEST_BOOL(v.data[1] == 2);
        }
    }
    {
        // operators
        {
            // ==
            {
                Vector2f v1 = {1.5, 2.5};
                Vector2f v2 = { 1.5, 2.5 };
                TEST_BOOL(v1 == v2);
            }
            {
                Vector2f v1 = {1.5, 2.50001};
                Vector2f v2 = { 1.5, 2.5 };
                TEST_BOOL(!(v1 == v2));
            }
        }
        {
            // !=
            {
                Vector2f v1 = {1.5, 2.5};
                Vector2f v2 = { 1.58, 2.5 };
                TEST_BOOL(v1 != v2);
            }
            {
                Vector2f v1 = {1.5, 2.5};
                Vector2f v2 = { 1.5, 2.51 };
                TEST_BOOL(v1 != v2);
            }
        }
        {
            // +=
            Vector2f v1 = { 1.5, 2 };
            constexpr Vector2f v2 = { 3.5, 2.5 };
            v1 += v2;
            TEST_BOOL(tMath::approximately(v1, { 5, 4.5 }));
        }
        {
            // -=
            Vector2f v1 = { 1.5, 2 };
            constexpr Vector2f v2 = { 3.5, 2.5 };
            v1 -= v2;
            TEST_BOOL(tMath::approximately(v1, { -2, -0.5 }));
        }
        {
            // *=
            Vector2f v1 = { 1.5, 2 };
            const float f = 3;
            v1 *= f;
            TEST_BOOL(tMath::approximately(v1, { 4.5, 6 }));
        }
        {
            // /=
            Vector2f v1 = { 1.5, 2 };
            const float f = 3;
            v1 /= f;
            TEST_BOOL(tMath::approximately(v1, { 0.5, 2.0/3.0 }));
        }
        {
            // safe /=
            Vector2f v1 = {1.5, 2};
            const float zero = 0;
            tMath::safe_divide_inplace(v1, zero, {5, 6});
            TEST_BOOL(tMath::approximately({5, 6}, v1));
        }
        {
            // +
            Vector2f v1 = { 1.5, 2 };
            Vector2f v2 = { 3.5, 2.5 };
            TEST_BOOL(tMath::approximately(v1 + v2, { 5, 4.5 }));
        }
        {
            // -
            Vector2f v1 = { 1.5, 2 };
            constexpr Vector2f v2 = { 3.5, 2.5 };
            TEST_BOOL(tMath::approximately(v1 - v2, { -2, -0.5 }));
        }
        {
            // *
            Vector2f v1 = { 1.5, 2 };
            const float f = 3;
            TEST_BOOL(tMath::approximately(v1 * f, { 4.5, 6 }));
        }
        {
            // /
            Vector2f v1 = { 1.5, 2 };
            const float f = 3;
            TEST_BOOL(tMath::approximately(v1 / f, { 0.5, 2.0/3.0 }));
        }
        {
            // safe /
            Vector2f v = {1.5, 2};
            const float zero = 0;
            TEST_BOOL(tMath::approximately(tMath::safe_divide(v, zero, {5, 6}), {5, 6}));
        }
        {
            // dot
            {
                Vector2f v1 = { 1, 2 };
                Vector2f v2 = { 2, 3 };
                float f = tMath::dot(v1.x, v1.y, v2.x, v2.y);
                TEST_BOOL(tMath::approximately(f, 8.0f));
            }
            {
                Vector2f v1 = { 1, 2 };
                Vector2f v2 = { 2, 3 };
                float f = tMath::dot(v1, v2);
                TEST_BOOL(tMath::approximately(f, 8.0f));
            }
        }
        {
            // fake cross
            {
                // v1 -> v2 是顺时针
                Vector2f v1 = { -1, 1 };
                Vector2f v2 = { 1, 1 };
                TEST_BOOL(tMath::cross(v1, v2) < 0);
            }
            {
                // v1 -> v2 是逆时针
                Vector2f v1 = { -1, 1 };
                Vector2f v2 = { 1, 1 };
                TEST_BOOL(tMath::cross(v2, v1) > 0);
            }
            {
                // v1, v2平行
                Vector2f v1 = { -1, -1 };
                Vector2f v2 = { 1, 1 };
                TEST_BOOL(tMath::cross(v1, v2) == 0);
            }

            {
                // v1 -> v2 是顺时针
                Vector2f v1 = { -1, 1 };
                Vector2f v2 = { 1, 1 };
                TEST_BOOL(tMath::cross(v1.x, v1.y, v2.x, v2.y) < 0);
            }
            {
                // v1 -> v2 是逆时针
                Vector2f v1 = { -1, 1 };
                Vector2f v2 = { 1, 1 };
                TEST_BOOL(tMath::cross(v2.x, v2.y, v1.x, v1.y) > 0);
            }
            {
                // v1, v2平行
                Vector2f v1 = { -1, -1 };
                Vector2f v2 = { 1, 1 };
                TEST_BOOL(tMath::cross(v1.x, v1.y, v2.x, v2.y) == 0);
            }
        }
        {
            // casts
            {
                Vector2f v2f = { 1.1, 2.2 };
                Vector2i16 test = { 1, 2 };
                TEST_BOOL(tMath::vector_cast<Vector2i16>(v2f) == test);
            }
            {
                const float x = 1.111111f;
                const float y = 2.222222f;
                Vector2f v2f = { x, y };
                Vector2d test = { static_cast<double>(x), static_cast<double>(y) };
                TEST_BOOL(tMath::approximately(tMath::vector_cast<Vector2d>(v2f), test));
                TEST_BOOL(tMath::vector_cast<Vector2d>(v2f) == test);
            }
        }
    }
    {
        Vector2f v2{};
        TEST_BOOL(tMath::approximately(v2, {0, 0}));
    }
    {
        Vector2f degrees = { 180, 180 };
        TEST_BOOL(tMath::approximately(tMath::to_radians(degrees), { tMath::PI<float>, tMath::PI<float> }));
    }
    {
        Vector2f radians = { 2 * tMath::PI<float>, tMath::PI<float> };
        TEST_BOOL(tMath::approximately(tMath::to_degrees(radians), { 360, 180 }));
    }
    {
        Vector2f negative = { -180, -3 };
        TEST_BOOL(tMath::approximately(tMath::abs(negative), { 180, 3 }));
    }
    {
        {
            Vector2f vec = { -1, 1 };
            auto f = tMath::magnitude(vec);
            TEST_BOOL(tMath::approximately(f, 1.414213f));
        }
        {
            // Vector2f vec = { -1, 1 };
            auto f = tMath::magnitude(-1.0f, 1.0f);
            TEST_BOOL(tMath::approximately(f, 1.414213f));
        }
    }
    {
        Vector2f vec = { 1000, 0 };
        TEST_BOOL(tMath::approximately(tMath::normalized(vec), { 1, 0 }));
    }
    {
        Vector2f vec = {1000, 0};
        tMath::normalize_inplace(vec);
        TEST_BOOL(tMath::approximately(vec, { 1, 0 }));
    }
    {
        // sint normalized
        Vector2i32 vi = { 1234, 1234 };
        TEST_BOOL(tMath::approximately(tMath::normalized<Vector2f>(vi), { 0.707106f, 0.707106f }));
    }
    {
        // distance
        {
            // f32
            Vector2f v1 = { 5, 10 };
            Vector2f v2 = { 2, -5 };
            auto dis = tMath::distance(v1, v2);
            TEST_BOOL(tMath::approximately(dis, 15.297058f));
        }
        {
            // i32
            Vector2i32 v1 = { 5, 10 };
            Vector2i32 v2 = { 2, -5 };
            auto dis = tMath::distance(v1, v2); // int32 返回的是 double
            TEST_BOOL(tMath::approximately(dis, 15.29705854077835449));
        }
        {
            // i16
            Vector2i16 v1 = { 5, 10 };
            Vector2i16 v2 = { 2, -5 };
            auto dis = tMath::distance(v1, v2); // int16 返回的是 float
            TEST_BOOL(tMath::approximately(dis, 15.297058f));
        }
    }
    {
        // lerp
        Vector2f a = { -10, 10 };
        Vector2f b = { 10, 30 };
        TEST_BOOL(tMath::approximately(tMath::lerp(a, b, 0.6f), { 2.0f, 22.0f }));
    }

    {
        // zero divide
        {
            Vector2f zero = {0, 0};
            Vector2f n = tMath::safe_normalized(zero, {5, 5});
            TEST_BOOL(tMath::approximately(n, {5, 5}));
        }
        {
            Vector2i32 zero = {0, 0};
            Vector2f n = tMath::safe_normalized<Vector2f>(zero, {10, 5});
            TEST_BOOL(tMath::approximately(n, {10, 5}));
        }
        {
            Vector2f zero = {0, 0};
            tMath::safe_normalize_inplace(zero, {5, 10});
            TEST_BOOL(tMath::approximately(zero, {5, 10}));
        }
        {
            // safe divide
            Vector2f v = {10, 10};
            float zero = 0;
            TEST_BOOL(tMath::approximately(tMath::safe_divide(v, zero, {100, 100}), {100, 100}));
        }
        {
            Vector2f v = {10, 10};
            float zero = 0;
            tMath::safe_divide_inplace(v, zero, {100, 100});
            TEST_BOOL(tMath::approximately(v, {100, 100}));
        }
    }

    TEST_END("test vector2");
}

void test_vector3()
{
    TEST_TITLE("test vector3");
    {
        Vector3f v{1, 2, 3};
        TEST_BOOL(v[0] == 1);
        TEST_BOOL(v[1] == 2);
        TEST_BOOL(v[2] == 3);
        TEST_BOOL(v.x == 1);
        TEST_BOOL(v.y == 2);
        TEST_BOOL(v.z == 3);
        TEST_BOOL(v.r == 1);
        TEST_BOOL(v.g == 2);
        TEST_BOOL(v.b == 3);
        TEST_BOOL(v.data[0] == 1);
        TEST_BOOL(v.data[1] == 2);
        TEST_BOOL(v.data[2] == 3);
    }
    {
        // operators
        {
            // ==
            {
                Vector3f v1 = {1.5, 2.5, 5.5};
                Vector3f v2 = { 1.5, 2.5, 5.5 };
                TEST_BOOL(v1 == v2);
            }
            {
                Vector3f v1 = {1.5, 2.50001, 1};
                Vector3f v2 = { 1.5, 2.5, 1 };
                TEST_BOOL(!(v1 == v2));
            }
        }
        {
            // !=
            {
                Vector3f v1 = {1.5, 2.5, 5.51};
                Vector3f v2 = { 1.5, 2.5, 5.5 };
                TEST_BOOL(v1 != v2);
            }
        }
        {
            // +=
            Vector3f v1 = { 1.5, 2, 5.5 };
            constexpr Vector3f v2 = { 3.5, 2.5, 5.5 };
            v1 += v2;
            TEST_BOOL(tMath::approximately(v1, { 5, 4.5, 11.0 }));
        }
        {
            // -=
            Vector3f v1 = { 1.5, 2, 5.5 };
            constexpr Vector3f v2 = { 3.5, 2.5, 15.2 };
            v1 -= v2;
            TEST_BOOL(tMath::approximately(v1, { -2, -0.5, -9.7 }));
        }
        {
            // *=
            Vector3f v1 = { 1.5, 2, 5.5 };
            const float f = 3;
            v1 *= f;
            TEST_BOOL(tMath::approximately(v1, { 4.5, 6, 16.5 }));
        }
        {
            // /=
            Vector3f v1 = { 1.5, 2, 5.5 };
            const float f = 3;
            v1 /= f;
            TEST_BOOL(tMath::approximately(v1, { 0.5, 2.0/3.0, 5.5/3.0 }));
        }
        {
            // +
            Vector3f v1 = { 1.5, 2, 5.5 };
            Vector3f v2 = { 3.5, 2.5, 5.5 };
            TEST_BOOL(tMath::approximately(v1 + v2, { 5, 4.5, 11.0 }));
        }
        {
            // -
            Vector3f v1 = { 1.5, 2, 15.5 };
            constexpr Vector3f v2 = { 3.5, 2.5, 5.2 };
            TEST_BOOL(tMath::approximately(v1 - v2, { -2, -0.5, 10.3 }));
        }
        {
            // *
            Vector3f v1 = { 1.5, 2, 5.5 };
            const float f = 3;
            TEST_BOOL(tMath::approximately(v1 * f, { 4.5, 6, 16.5 }));
        }
        {
            // /
            Vector3f v1 = { 1.5, 2, 5.5 };
            const float f = 3;
            TEST_BOOL(tMath::approximately(v1 / f, { 0.5, 2.0/3.0, 5.5/3.0 }));
        }
        {
            // dot
            {
                Vector3f v1 = { 1, 2, 3 };
                Vector3f v2 = { 2, 3, 4 };
                float f = tMath::dot(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
                TEST_BOOL(tMath::approximately(f, 20.0f));
            }
            {
                Vector3f v1 = { 1, 2, 3 };
                Vector3f v2 = { 2, 3, 4 };
                float f = tMath::dot(v1, v2);
                TEST_BOOL(tMath::approximately(f, 20.0f));
            }
        }
        {
            // cross
            Vector3f v1 = { -1, 1, 6 };
            Vector3f v2 = { 1, 1, -10 };
            TEST_BOOL(tMath::approximately(tMath::cross(v1, v2), { -16, -4, -2 }));
        }

        {
            // casts
            {
                Vector3f v3f = { 1.1, 2.2, 3.3 };
                Vector3i16 test = { 1, 2, 3 };
                TEST_BOOL(tMath::vector_cast<Vector3i16>(v3f) == test);
            }
            {
                const float x = 1.111111f;
                const float y = 2.222222f;
                const float z = 3.333333f;
                Vector3f v3f = { x, y, z };
                Vector3d test = { static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
                TEST_BOOL(tMath::approximately(tMath::vector_cast<Vector3d>(v3f), test));
                TEST_BOOL(tMath::vector_cast<Vector3d>(v3f) == test);
            }
        }
    }

    {
        Vector3f v3{};
        TEST_BOOL(tMath::approximately(v3, {0, 0, 0}));
    }
    {
        // to degrees
        Vector3f degrees = { 180, 180, 360 };
        TEST_BOOL(tMath::approximately(tMath::to_radians(degrees), { tMath::PI<float>, tMath::PI<float>, tMath::PI<float> * 2 }));
    }
    {
        Vector3f radians = { 2 * tMath::PI<float>, tMath::PI<float>, tMath::PI<float> };
        TEST_BOOL(tMath::approximately(tMath::to_degrees(radians), { 360, 180, 180 }));
    }
    {
        Vector3f negative = { -180, -3, -150.123f };
        TEST_BOOL(tMath::approximately(tMath::abs(negative), { 180, 3, 150.123f }));
    }
    {
        {
            Vector3f vec = { -1, 1, -1 };
            auto f = tMath::magnitude(vec);
            TEST_BOOL(tMath::approximately(f, 1.732050f));
        }
        {
            // Vector3f vec = { -1, 1, -1 };
            auto f = tMath::magnitude(-1.0f, 1.0f, -1.0f);
            TEST_BOOL(tMath::approximately(f, 1.732050f));
        }
    }
    {
        Vector3f vec = { 1000, 0, 1000 };
        TEST_BOOL(tMath::approximately(tMath::normalized(vec), { 0.707106f, 0, 0.707106f }));
    }
    {
        Vector3f vec = { 1000, 0, 1000 };
        tMath::normalize_inplace(vec);
        TEST_BOOL(tMath::approximately(vec, { 0.707106f, 0, 0.707106f }));
    }
    {
        // sint normalized
        Vector3i32 vi = { 1234, 1234, 1234 };
        TEST_BOOL(tMath::approximately(tMath::normalized<Vector3f>(vi), { 0.577350f, 0.577350f, 0.577350f }));
    }
    {
        // distance
        {
            // f32
            Vector3f v1 = { 5, 10, 5 };
            Vector3f v2 = { 2, -5, 5 };
            auto dis = tMath::distance(v1, v2);
            TEST_BOOL(tMath::approximately(dis, 15.297058f));
        }
        {
            // i32
            Vector3i32 v1 = { 5, 10, 9 };
            Vector3i32 v2 = { 2, -5, 9 };
            auto dis = tMath::distance(v1, v2); // int32 返回的是 double
            TEST_BOOL(tMath::approximately(dis, 15.29705854077835449));
        }
        {
            // i16
            Vector3i16 v1 = { 5, 10, 90 };
            Vector3i16 v2 = { 2, -5, 90 };
            auto dis = tMath::distance(v1, v2); // int16 返回的是 float
            TEST_BOOL(tMath::approximately(dis, 15.297058f));
        }
    }
    {
        // lerp
        Vector3f a = { -10, 10, -10 };
        Vector3f b = { 10, 30, 10 };
        TEST_BOOL(tMath::approximately(tMath::lerp(a, b, 0.6f), { 2.0f, 22.0f, 2.0f }));
    }

    {
        // zero divide
        {
            Vector3f zero = {0, 0};
            Vector3f n = tMath::safe_normalized(zero, {5, 5, 1});
            TEST_BOOL(tMath::approximately(n, {5, 5, 1}));
        }
        {
            Vector3i32 zero = {0, 0, 0};
            Vector3f n = tMath::safe_normalized<Vector3f>(zero, {10, 5, 6});
            TEST_BOOL(tMath::approximately(n, {10, 5, 6}));
        }
        {
            Vector3f zero = {0, 0, 0};
            tMath::safe_normalize_inplace(zero, {5, 10, -5});
            TEST_BOOL(tMath::approximately(zero, {5, 10, -5}));
        }
        {
            // safe divide
            Vector3f v = {10, 10, 10};
            float zero = 0;
            TEST_BOOL(tMath::approximately(tMath::safe_divide(v, zero, {100, 100, 100}), {100, 100, 100}));
        }
        {
            Vector3f v = {10, 10, 10};
            float zero = 0;
            tMath::safe_divide_inplace(v, zero, {100, 100, 98});
            TEST_BOOL(tMath::approximately(v, {100, 100, 98}));
        }
    }

    TEST_END("test vector3");
}

void test_vector4()
{
    TEST_TITLE("test vector4");

    {
        Vector4f v{1, 2, 3, 4};
        TEST_BOOL(v[0] == 1);
        TEST_BOOL(v[1] == 2);
        TEST_BOOL(v[2] == 3);
        TEST_BOOL(v[3] == 4);
        TEST_BOOL(v.x == 1);
        TEST_BOOL(v.y == 2);
        TEST_BOOL(v.z == 3);
        TEST_BOOL(v.w == 4);
        TEST_BOOL(v.r == 1);
        TEST_BOOL(v.g == 2);
        TEST_BOOL(v.b == 3);
        TEST_BOOL(v.a == 4);
        TEST_BOOL(v.data[0] == 1);
        TEST_BOOL(v.data[1] == 2);
        TEST_BOOL(v.data[2] == 3);
        TEST_BOOL(v.data[3] == 4);
    }
    {
        // quat tag
        TEST_BOOL(tMath::is_vector4_float<Vector4f>);
        TEST_BOOL(!tMath::is_quat<Vector4f>);
    }

    {
        // operators
        {
            // ==
            {
                Vector4f v1 = { 1.5, 2.5, 5.5, 6.6 };
                Vector4f v2 = { 1.5, 2.5, 5.5, 6.6 };
                TEST_BOOL(v1 == v2);
            }
            {
                Vector4f v1 = {1, 2, 1, 1.1};
                Vector4f v2 = { 1, 2, 1, 1.2 };
                TEST_BOOL(!(v1 == v2));
            }
        }
        {
            // !=
            {
                Vector4f v1 = {1.5, 2.5, 5.5, 1.1};
                Vector4f v2 = { 1.5, 2.5, 5.5, 1.2 };
                TEST_BOOL(v1 != v2);
            }
        }
        {
            // +=
            Vector4f v1 = { 1.5, 2, 5.5, 1 };
            Vector4f v2 = { 3.5, 2.5, 5.5, 5.678 };
            Vector4f v3 = { 1, 2, 3, 4 };
            v1 += v2 += v3;
            TEST_BOOL(tMath::approximately(v1, { 6, 6.5, 14.0, 10.678 }));
        }
        {
            // -=
            Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
            Vector4f v2 = { 3.5, 2.5, 15.2, 15.2 };
            Vector4f v3 = { 1, 2, 3, 4 };
            v1 -= v2 -= v3;
            TEST_BOOL(tMath::approximately(v1, { -1, 1.5, -6.7, -5.7 }));
        }
        {
            // *=
            Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
            float f = 3;
            v1 *= f *= 3;
            TEST_BOOL(tMath::approximately(v1, { 13.5, 18, 49.5, 49.5 }));
        }
        {
            // /=
            Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
            float f = 3;
            v1 /= f /= 2;
            TEST_BOOL(tMath::approximately(v1, { 1, 2.0/1.5, 5.5/1.5, 5.5/1.5 }));
        }
        {
            // +
            Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
            Vector4f v2 = { 3.5, 2.5, 5.5, 5.5 };
            TEST_BOOL(tMath::approximately(v1 + v2, { 5, 4.5, 11.0, 11.0 }));
        }
        {
            // -
            Vector4f v1 = { 1.5, 2, 15.5, 15.5 };
            constexpr Vector4f v2 = { 3.5, 2.5, 5.2, 5.2 };
            TEST_BOOL(tMath::approximately(v1 - v2, { -2, -0.5, 10.3, 10.3 }));
        }
        {
            // *
            Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
            const float f = 3;
            TEST_BOOL(tMath::approximately(v1 * f, { 4.5, 6, 16.5, 16.5 }));
        }
        {
            // /
            Vector4f v1 = { 1.5, 2, 5.5, 5.5 };
            const float f = 3;
            TEST_BOOL(tMath::approximately(v1 / f, { 0.5, 2.0/3.0, 5.5/3.0, 5.5/3.0 }));
        }
        {
            // dot
            {
                Vector4f v1 = { 1, 2, 3, 3 };
                Vector4f v2 = { 2, 3, 4, 4 };
                float f = tMath::dot(v1.x, v1.y, v1.z, v1.w, v2.x, v2.y, v2.z, v2.w);
                TEST_BOOL(tMath::approximately(f, 32.0f));
            }
            {
                Vector4f v1 = { 1, 2, 3, 3 };
                Vector4f v2 = { 2, 3, 4, 4 };
                float f = tMath::dot(v1, v2);
                TEST_BOOL(tMath::approximately(f, 32.0f));
            }
        }
        {
            // cross
            Vector4f v1 = { -1, 1, 6, 5 };
            Vector4f v2 = { 1, 1, -10, 10 };
            TEST_BOOL(tMath::approximately(tMath::cross(v1, v2), { -16, -4, -2, 0 }));
        }

        {
            // casts
            {
                Vector4f v4f = { 1.1, 2.2, 3.3, 4.4 };
                Vector4i16 test = { 1, 2, 3, 4 };
                TEST_BOOL(tMath::vector_cast<Vector4i16>(v4f) == test);
            }
            {
                const float x = 1.111111f;
                const float y = 2.222222f;
                const float z = 3.333333f;
                const float w = 4.444444f;
                Vector4f v4f = { x, y, z, w };
                Vector4d test = { static_cast<double>(x), static_cast<double>(y), static_cast<double>(z), static_cast<double>(w) };
                TEST_BOOL(tMath::approximately(tMath::vector_cast<Vector4d>(v4f), test));
                TEST_BOOL(tMath::vector_cast<Vector4d>(v4f) == test);
            }
        }
    }

    {
        Vector4f v4{};
        TEST_BOOL(tMath::approximately(v4, {0, 0, 0, 0}));
    }
    {
        // to degrees
        Vector4f degrees = { 180, 180, 360, 90 };
        TEST_BOOL(tMath::approximately(tMath::to_radians(degrees), { tMath::PI<float>, tMath::PI<float>, tMath::PI<float> * 2, tMath::HalfPI<float> }));
    }
    {
        Vector4f radians = { 2 * tMath::PI<float>, tMath::PI<float>, tMath::PI<float>, tMath::HalfPI<float> };
        TEST_BOOL(tMath::approximately(tMath::to_degrees(radians), { 360, 180, 180, 90 }));
    }
    {
        Vector4f negative = { -180, -3, -150.123f, -256.12 };
        TEST_BOOL(tMath::approximately(tMath::abs(negative), { 180, 3, 150.123f, 256.12 }));
    }
    {
        // triangle functions
        {
            Vector4f v = { -180, -3, -150.123f, -256.12 };
            TEST_BOOL(tMath::approximately(tMath::sin(v), { sin(-180.0f), sin(-3.0f), sin(-150.123f), sin(-256.12f) }));
            TEST_BOOL(tMath::approximately(tMath::cos(v), { cos(-180.0f), cos(-3.0f), cos(-150.123f), cos(-256.12f) }));
            TEST_BOOL(tMath::approximately(tMath::tan(v), { tan(-180.0f), tan(-3.0f), tan(-150.123f), tan(-256.12f) }));
        }
    }
    {
        {
            Vector4f vec = { -1, 1, -1, 2 };
            auto f = tMath::magnitude(vec);
            TEST_BOOL(tMath::approximately(f, 2.645751f));
        }
        {
            // Vector4f vec = { -1, 1, -1, 2 };
            auto f = tMath::magnitude(-1.0f, 1.0f, -1.0f, 2.0f);
            TEST_BOOL(tMath::approximately(f, 2.645751f));
        }
    }
    {
        Vector4f vec = { 1000, 0, 1000, 1000 };
        TEST_BOOL(tMath::approximately(tMath::normalized(vec), { 0.577350f, 0, 0.577350f, 0.577350f }));
    }
    {
        Vector4f vec = { 1000, 0, 1000, 1000 };
        tMath::normalize_inplace(vec);
        TEST_BOOL(tMath::approximately(vec, { 0.577350f, 0, 0.577350f, 0.577350f }));
    }
    {
        // sint normalized
        Vector4i32 vi = { 1234, 1234, 1234, 1234 };
        TEST_BOOL(tMath::approximately(tMath::normalized<Vector4f>(vi), { 0.5f, 0.5f, 0.5f, 0.5f }));
    }
    {
        // distance
        {
            // f32
            Vector4f v1 = { 5, 10, 5, 10 };
            Vector4f v2 = { 2, 10, 5, -5 };
            auto dis = tMath::distance(v1, v2);
            TEST_BOOL(tMath::approximately(dis, 15.297058f));
        }
        {
            // i32
            Vector4i32 v1 = { 5, 10, 9, 10 };
            Vector4i32 v2 = { 2, 10, 9, -5 };
            auto dis = tMath::distance(v1, v2); // int32 返回的是 double
            TEST_BOOL(tMath::approximately(dis, 15.29705854077835449));
        }
        {
            // i16
            Vector4i16 v1 = { 5, 10, 90 ,10 };
            Vector4i16 v2 = { 2, 10, 90, -5 };
            auto dis = tMath::distance(v1, v2); // int16 返回的是 float
            TEST_BOOL(tMath::approximately(dis, 15.297058f));
        }
    }
    {
        // lerp
        Vector4f a = { -10, 10, -10, -10 };
        Vector4f b = { 10, 30, 10, 10 };
        TEST_BOOL(tMath::approximately(tMath::lerp(a, b, 0.6f), { 2.0f, 22.0f, 2.0f, 2.0f }));
    }

    {
        // zero divide
        {
            Vector4f zero = {0, 0, 0, 0};
            Vector4f n = tMath::safe_normalized(zero, {5, 5, 1, 6});
            TEST_BOOL(tMath::approximately(n, {5, 5, 1, 6}));
        }
        {
            Vector4i32 zero = {0, 0, 0, 0};
            Vector4f n = tMath::safe_normalized<Vector4f>(zero, {10, 5, 6, 10});
            TEST_BOOL(tMath::approximately(n, {10, 5, 6, 10}));
        }
        {
            Vector4f zero = {0, 0, 0 , 0};
            tMath::safe_normalize_inplace(zero, {5, 10, -5, -8});
            TEST_BOOL(tMath::approximately(zero, {5, 10, -5, -8}));
        }
        {
            // safe divide
            Vector4f v = {10, 10, 10, 10};
            float zero = 0;
            TEST_BOOL(tMath::approximately(tMath::safe_divide(v, zero, {100, 100, 100, 99}), {100, 100, 100, 99}));
        }
        {
            Vector4f v = {10, 10, 10, 5};
            float zero = 0;
            tMath::safe_divide_inplace(v, zero, {100, 100, 98, 97});
            TEST_BOOL(tMath::approximately(v, {100, 100, 98, 97}));
        }
    }

    TEST_END("test vector4");
}

int main()
{
    try
    {
        test_vector2();
        test_vector3();
        test_vector4();
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }

    std::cout << "SUCCEED (test_vector)" << std::endl;

    return EXIT_SUCCESS;
}