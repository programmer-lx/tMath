#pragma once

#include "test_types.hpp"

#include "../test.hpp"

using tSimd::float32_4;

TEST(float32_4, store)
{
    {
        float32_4 v = tSimd::load_point(Vector2f{2, 3});
        Vector2f r{};
        tSimd::store(r, v);
        EXPECT_TRUE(r.x == 2);
        EXPECT_TRUE(r.y == 3);
    }
    {
        float32_4 v = tSimd::load_point(Vector2f{2, 3});
        Vector4f r{};
        tSimd::store(r, v);
        EXPECT_TRUE(r.x == 2);
        EXPECT_TRUE(r.y == 3);
        EXPECT_TRUE(r.z == 0);
        EXPECT_TRUE(r.w == 1);
    }
    {
        float32_4 v = tSimd::load_point(Vector3f{2, 3, 4});
        Vector3f r{};
        tSimd::store(r, v);
        EXPECT_TRUE(r.x == 2);
        EXPECT_TRUE(r.y == 3);
        EXPECT_TRUE(r.z == 4);
    }
    {
        float32_4 v = tSimd::load_point(Vector3f{2, 3, 4});
        Vector4f r{};
        tSimd::store(r, v);
        EXPECT_TRUE(r.x == 2);
        EXPECT_TRUE(r.y == 3);
        EXPECT_TRUE(r.z == 4);
        EXPECT_TRUE(r.w == 1);
    }
    {
        float32_4 v = tSimd::load(1, 2, 3, 4);
        Vector4f v4f{};
        tSimd::store(v4f, v);
        EXPECT_TRUE(tMath::approximately(v4f, {1, 2, 3, 4}));
    }
}

TEST(float32_4, fields)
{
    Vector4f v4f = { 1, 2, 3, 4 };
    float32_4 v = tSimd::load(v4f);

    Vector4f test{};
    tSimd::store(test, v);

    EXPECT_TRUE(tMath::approximately(test, {1, 2, 3, 4}));

    EXPECT_TRUE(tSimd::get_x(v) == 1);
    EXPECT_TRUE(tSimd::get_y(v) == 2);
    EXPECT_TRUE(tSimd::get_z(v) == 3);
    EXPECT_TRUE(tSimd::get_w(v) == 4);
}

TEST(float32_4, set)
{
    // set all
    float32_4 v = tSimd::load(2);
    Vector4f test{};
    tSimd::store(test, v);
    EXPECT_TRUE(tMath::approximately(test, {2, 2, 2, 2}));
}

TEST(float32_4, add)
{
    float32_4 v = tSimd::load(1, 2, 3, 4);
    float32_4 v2 = tSimd::load(1);
    float32_4 result = tSimd::add(v, v2);

    Vector4f test{};
    tSimd::store(test, result);

    EXPECT_TRUE(tMath::approximately(test, {2, 3, 4, 5}));
}

TEST(float32_4, sub)
{
    float32_4 v = tSimd::load(1, 2, 3, 4);
    float32_4 v2 = tSimd::load(1);
    float32_4 result = tSimd::sub(v, v2);

    Vector4f test{};
    tSimd::store(test, result);

    EXPECT_TRUE(tMath::approximately(test, {0, 1, 2, 3}));
}

TEST(float32_4, mul)
{
    {
        float32_4 v = tSimd::load(1, 2, 3, 4);
        float32_4 v2 = tSimd::load(2);
        float32_4 result = tSimd::mul(v, v2);

        Vector4f test{};
        tSimd::store(test, result);

        EXPECT_TRUE(tMath::approximately(test, {2, 4, 6, 8}));
    }
    {
        float32_4 v = tSimd::load(1, 2, 3, 4);
        float32_4 result = tSimd::mul(v, 2);

        Vector4f test{};
        tSimd::store(test, result);

        EXPECT_TRUE(tMath::approximately(test, {2, 4, 6, 8}));
    }
}

TEST(float32_4, div)
{
    {
        float32_4 v = tSimd::load(1, 2, 3, 4);
        float32_4 v2 = tSimd::load(2);
        float32_4 result = tSimd::div(v, v2);

        Vector4f test{};
        tSimd::store(test, result);

        EXPECT_TRUE(tMath::approximately(test, {0.5, 1, 1.5, 2}));
    }
    {
        float32_4 v = tSimd::load(1, 2, 3, 4);
        float32_4 result = tSimd::div(v, 2);

        Vector4f test{};
        tSimd::store(test, result);

        EXPECT_TRUE(tMath::approximately(test, {0.5, 1, 1.5, 2}));
    }
}

TEST(float32_4, mul_add)
{
    // mul add
    float32_4 a = tSimd::load(1, 2, 3, 4);
    float32_4 b = tSimd::load(2, 3, 4, 5);
    float32_4 c = tSimd::load(3, 4, 5, 6);
    float32_4 result = tSimd::mul_add(a, b, c);

    Vector4f v4f{};
    tSimd::store(v4f, result);
    EXPECT_TRUE(tMath::approximately(v4f, { 5, 10, 17, 26 }));
}

TEST(float32_4, dot2)
{
    // dot2
    for (int i = 0; i < 500; ++i)
    {
        float x1 = random_f(-10.0f, 10.0f);
        float y1 = random_f(-10.0f, 10.0f);
        float z1 = random_f(-10.0f, 10.0f);
        float w1 = random_f(-10.0f, 10.0f);

        float x2 = random_f(-10.0f, 10.0f);
        float y2 = random_f(-10.0f, 10.0f);
        float z2 = random_f(-10.0f, 10.0f);
        float w2 = random_f(-10.0f, 10.0f);

        float32_4 a = tSimd::load(x1, y1, z1, w1);
        float32_4 b = tSimd::load(x2, y2, z2, w2);
        double test =
            double(x1)*x2 +
            double(y1)*y2;

        float32_4 result = tSimd::dot2(a, b);
        Vector4f r{};
        tSimd::store(r, result);
        EXPECT_TRUE(tMath::approximately(r.x, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.y, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.z, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.w, float(test), 1e-4f));
    }
}

TEST(float32_4, dot3)
{
    // dot3
    for (int i = 0; i < 500; ++i)
    {
        float x1 = random_f(-10.0f, 10.0f);
        float y1 = random_f(-10.0f, 10.0f);
        float z1 = random_f(-10.0f, 10.0f);
        float w1 = random_f(-10.0f, 10.0f);

        float x2 = random_f(-10.0f, 10.0f);
        float y2 = random_f(-10.0f, 10.0f);
        float z2 = random_f(-10.0f, 10.0f);
        float w2 = random_f(-10.0f, 10.0f);

        float32_4 a = tSimd::load(x1, y1, z1, w1);
        float32_4 b = tSimd::load(x2, y2, z2, w2);
        double test =
            double(x1)*x2 +
            double(y1)*y2 +
            double(z1)*z2;

        float32_4 result = tSimd::dot3(a, b);
        Vector4f r{};
        tSimd::store(r, result);
        EXPECT_TRUE(tMath::approximately(r.x, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.y, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.z, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.w, float(test), 1e-4f));
    }
}

TEST(float32_4, dot4)
{
    // dot4
    for (int i = 0; i < 500; ++i)
    {
        float x1 = random_f(-10.0f, 10.0f);
        float y1 = random_f(-10.0f, 10.0f);
        float z1 = random_f(-10.0f, 10.0f);
        float w1 = random_f(-10.0f, 10.0f);

        float x2 = random_f(-10.0f, 10.0f);
        float y2 = random_f(-10.0f, 10.0f);
        float z2 = random_f(-10.0f, 10.0f);
        float w2 = random_f(-10.0f, 10.0f);

        float32_4 a = tSimd::load(x1, y1, z1, w1);
        float32_4 b = tSimd::load(x2, y2, z2, w2);
        double test =
            double(x1)*x2 +
            double(y1)*y2 +
            double(z1)*z2 +
            double(w1)*w2;

        float32_4 result = tSimd::dot4(a, b);
        Vector4f r{};
        tSimd::store(r, result);
        EXPECT_TRUE(tMath::approximately(r.x, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.y, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.z, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.w, float(test), 1e-4f));
    }
}

TEST(float32_4, cross2)
{
    // cross2
    for (int i = 0; i < 500; ++i)
    {
        float x1 = random_f(-10.0f, 10.0f);
        float y1 = random_f(-10.0f, 10.0f);
        float z1 = random_f(-10.0f, 10.0f);
        float w1 = random_f(-10.0f, 10.0f);

        float x2 = random_f(-10.0f, 10.0f);
        float y2 = random_f(-10.0f, 10.0f);
        float z2 = random_f(-10.0f, 10.0f);
        float w2 = random_f(-10.0f, 10.0f);

        float32_4 a = tSimd::load(x1, y1, z1, w1);
        float32_4 b = tSimd::load(x2, y2, z2, w2);
        double test = x1 * y2 - x2 * y1;

        float32_4 result = tSimd::cross2(a, b);
        Vector4f r{};
        tSimd::store(r, result);
        EXPECT_TRUE(tMath::approximately(r.x, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.y, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.z, float(test), 1e-4f));
        EXPECT_TRUE(tMath::approximately(r.w, float(test), 1e-4f));
    }
}

TEST(float32_4, abs)
{
    // abs
    float32_4 negative = tSimd::load(-1, -2, -3, -4);
    float32_4 result = tSimd::abs(negative);
    Vector4f test{};
    tSimd::store(test, result);
    EXPECT_TRUE(tMath::approximately(test, {1, 2, 3, 4}));
}

TEST(float32_4, sqrt)
{
    // sqrt
    float32_4 v = tSimd::load(1, 2, 3, 4);
    float32_4 result = tSimd::sqrt(v);
    Vector4f test{};
    tSimd::store(test, result);
    EXPECT_TRUE(tMath::approximately(test, {1, std::sqrt(2.0f), std::sqrt(3.0f), std::sqrt(4.0f)}));
}
