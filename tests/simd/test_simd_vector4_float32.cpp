#include <tMath/simd/float4.hpp>
#include <tMath/vector4.hpp>

#include "../test.hpp"

void check_simd_support()
{
}

struct Vector4f
{
    TMATH_VECTOR4(Vector4f, float)
};

void test()
{
    using tSimd::float4;

    {
        float4 v = tSimd::set(1);
        Vector4f v4f{};
        tSimd::store(v4f.data, v);
        TEST_BOOL(tMath::approximately(v4f, {1, 1, 1, 1}));
    }
    {
        float4 v = tSimd::set(1, 2, 3, 4);
        Vector4f v4f{};
        tSimd::store(v4f.data, v);
        TEST_BOOL(tMath::approximately(v4f, {1, 2, 3, 4}));
    }
    {
        float4 v = tSimd::set(1, 2, 3, 4);
        Vector4f v4f{};
        tSimd::store(v4f, v);
        TEST_BOOL(tMath::approximately(v4f, {1, 2, 3, 4}));
    }
    {
        Vector4f v4f = { 1, 2, 3, 4 };
        float4 v = tSimd::load(v4f);

        Vector4f test{};
        tSimd::store(test, v);

        TEST_BOOL(tMath::approximately(test, {1, 2, 3, 4}));
    }
    {
        // set all
        float4 v = tSimd::set(2);
        Vector4f test{};
        tSimd::store(test, v);
        TEST_BOOL(tMath::approximately(test, {2, 2, 2, 2}));
    }
    {
        // operators
        {
            // +
            {
                float4 v = tSimd::set(1, 2, 3, 4);
                float4 v2 = tSimd::set(1);
                float4 result = tSimd::add(v, v2);

                Vector4f test{};
                tSimd::store(test, result);

                TEST_BOOL(tMath::approximately(test, {2, 3, 4, 5}));
            }
            // -
            {
                float4 v = tSimd::set(1, 2, 3, 4);
                float4 v2 = tSimd::set(1);
                float4 result = tSimd::sub(v, v2);

                Vector4f test{};
                tSimd::store(test, result);

                TEST_BOOL(tMath::approximately(test, {0, 1, 2, 3}));
            }
            // *
            {
                {
                    float4 v = tSimd::set(1, 2, 3, 4);
                    float4 v2 = tSimd::set(2);
                    float4 result = tSimd::mul(v, v2);

                    Vector4f test{};
                    tSimd::store(test, result);

                    TEST_BOOL(tMath::approximately(test, {2, 4, 6, 8}));
                }
                {
                    float4 v = tSimd::set(1, 2, 3, 4);
                    float4 result = tSimd::mul(v, 2);

                    Vector4f test{};
                    tSimd::store(test, result);

                    TEST_BOOL(tMath::approximately(test, {2, 4, 6, 8}));
                }
            }
            // /
            {
                {
                    float4 v = tSimd::set(1, 2, 3, 4);
                    float4 v2 = tSimd::set(2);
                    float4 result = tSimd::div(v, v2);

                    Vector4f test{};
                    tSimd::store(test, result);

                    TEST_BOOL(tMath::approximately(test, {0.5, 1, 1.5, 2}));
                }
                {
                    float4 v = tSimd::set(1, 2, 3, 4);
                    float4 result = tSimd::div(v, 2);

                    Vector4f test{};
                    tSimd::store(test, result);

                    TEST_BOOL(tMath::approximately(test, {0.5, 1, 1.5, 2}));
                }
            }
        }
        {
            // mul add
            float4 a = tSimd::set(1, 2, 3, 4);
            float4 b = tSimd::set(2, 3, 4, 5);
            float4 c = tSimd::set(3, 4, 5, 6);
            float4 result = tSimd::mul_add(a, b, c);

            Vector4f v4f{};
            tSimd::store(v4f, result);
            TEST_BOOL(tMath::approximately(v4f, { 5, 10, 17, 26 }));
        }
        {
            // dot
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

                float4 a = tSimd::set(x1, y1, z1, w1);
                float4 b = tSimd::set(x2, y2, z2, w2);
                double test =
                    double(x1)*x2 +
                    double(y1)*y2 +
                    double(z1)*z2 +
                    double(w1)*w2;

                float result = tSimd::dot4(a, b);
                TEST_BOOL(tMath::approximately(result, float(test), 1e-4f));
            }
        }
    }

    {
        // abs
        float4 negative = tSimd::set(-1, -2, -3, -4);
        float4 result = tSimd::abs(negative);
        Vector4f test{};
        tSimd::store(test, result);
        TEST_BOOL(tMath::approximately(test, {1, 2, 3, 4}));
    }
    {
        // sqrt
        float4 v = tSimd::set(1, 2, 3, 4);
        float4 result = tSimd::sqrt(v);
        Vector4f test{};
        tSimd::store(test, result);
        TEST_BOOL(tMath::approximately(test, {1, std::sqrt(2.0f), std::sqrt(3.0f), std::sqrt(4.0f)}));
    }
}

int main()
{
    try
    {
        check_simd_support();
        test();
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}