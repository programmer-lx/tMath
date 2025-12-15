#include <tMath/vector2.hpp>
#include <tMath/vector3.hpp>

#include "test.hpp"

struct Vector2f
{
    float x = 0, y = 0;
};
TMATH_REGISTER_VECTOR_TYPE(Vector2f)

struct Vector2d
{
    double x = 0, y = 0;
};
TMATH_REGISTER_VECTOR_TYPE(Vector2d)

struct Vector2i16
{
    int16_t x = 0, y = 0;
};
TMATH_REGISTER_VECTOR_TYPE(Vector2i16)

struct Vector2i32
{
    int32_t x = 0, y = 0;
};
TMATH_REGISTER_VECTOR_TYPE(Vector2i32)

struct Vector3f
{
    float x = 0, y = 0, z = 0;
};
TMATH_REGISTER_VECTOR_TYPE(Vector3f)

struct Vector3d
{
    double x = 0, y = 0, z = 0;
};
TMATH_REGISTER_VECTOR_TYPE(Vector3d)

struct Vector3i32
{
    int32_t x = 0, y = 0, z = 0;
};
TMATH_REGISTER_VECTOR_TYPE(Vector3i32)

struct Vector3i16
{
    int16_t x = 0, y = 0, z = 0;
};
TMATH_REGISTER_VECTOR_TYPE(Vector3i16)

struct Vector4f
{
    float x = 0, y = 0, z = 0, w = 0;
};
// TMATH_REGISTER_VECTOR_TYPE(Vector4f)

void test_vector2()
{
    TEST_TITLE("test vector2");

    {
        // copy function
        Vector2f v1 = {1, 1};
        Vector2f v2;
        v2 = v1;
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
            Vector2f v1 = { 1, 2 };
            Vector2f v2 = { 2, 3 };
            float f = tMath::dot(v1, v2);
            TEST_BOOL(tMath::approximately(f, 8.0f));
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
        }
        {
            // casts
            {
                Vector2f v2f = { 1.1, 2.2 };
                Vector2i16 test = { 1, 2 };
                TEST_BOOL(tMath::precision_cast<Vector2i16>(v2f) == test);
            }
            {
                const float x = 1.111111f;
                const float y = 2.222222f;
                Vector2f v2f = { x, y };
                Vector2d test = { static_cast<double>(x), static_cast<double>(y) };
                TEST_BOOL(tMath::approximately(tMath::precision_cast<Vector2d>(v2f), test));
                TEST_BOOL(tMath::precision_cast<Vector2d>(v2f) == test);
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
    }

    TEST_END("test vector2");
}

void test_vector3()
{
    TEST_TITLE("test vector3");

    {
        // operators TODO
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
                Vector3f v1 = {1.5, 2.5, 5.5};
                Vector3f v2 = { 1.58, 2.5, 5.5 };
                TEST_BOOL(v1 != v2);
            }
            {
                Vector3f v1 = {1.5, 2.5, 5.5};
                Vector3f v2 = { 1.5, 2.51, 5.5 };
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
            Vector3f v1 = { 1, 2, 3 };
            Vector3f v2 = { 2, 3, 4 };
            float f = tMath::dot(v1, v2);
            TEST_BOOL(tMath::approximately(f, 20.0f));
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
                TEST_BOOL(tMath::precision_cast<Vector3i16>(v3f) == test);
            }
            {
                const float x = 1.111111f;
                const float y = 2.222222f;
                const float z = 3.333333f;
                Vector3f v3f = { x, y, z };
                Vector3d test = { static_cast<double>(x), static_cast<double>(y), static_cast<double>(z) };
                TEST_BOOL(tMath::approximately(tMath::precision_cast<Vector3d>(v3f), test));
                TEST_BOOL(tMath::precision_cast<Vector3d>(v3f) == test);
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
        // distance TODO
    }

    TEST_END("test vector3");
}

void test_vector4()
{
    TEST_TITLE("test vector4");

    {
        // quat tag
        TEST_BOOL(tMath::is_vector4_float<Vector4f>);
        TEST_BOOL(!tMath::is_quat<Vector4f>);
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

    return EXIT_SUCCESS;
}