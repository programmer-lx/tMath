#include <../include/tMath/vector2.hpp>

#include "test.hpp"

TMATH_ENABLE_OPERATORS

struct Vector2f
{
    float x = 0, y = 0;
};
struct Vector2d
{
    double x = 0, y = 0;
};
struct Vector2i16
{
    int16_t x = 0, y = 0;
};
struct Vector2i32
{
    int32_t x = 0, y = 0;
};

struct Vector3
{
    float x = 0, y = 0, z = 0;
};

struct Vector4f
{
    float x = 0, y = 0, z = 0, w = 0;
};

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

    TEST_END("test vector2");
}

void test_vector3()
{

}

void test_vector4()
{
    TEST_TITLE("test vector4");

    {
        // quat tag
        TEST_BOOL(tMath::is_vector4_f32<Vector4f>);
        TEST_BOOL(!tMath::is_quat<Vector4f>);
    }

    TEST_END("test vector4");
}

int main()
{
    test_vector2();
    test_vector3();
    test_vector4();
    return 0;
}