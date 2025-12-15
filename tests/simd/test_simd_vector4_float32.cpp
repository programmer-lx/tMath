#include <tMath/simd/float32_4.hpp>
#include <tMath/vector4.hpp>

#include "../test.hpp"

void check_simd_support()
{
#if TMATH_SIMD_USE_SSE2
    TEST_BOOL(tSimd::supports_SSE2());
#else
    TEST_BOOL(tSimd::supports_SSE2() == false);
#endif

#if TMATH_SIMD_USE_AVX2
    TEST_BOOL(tSimd::supports_AVX2());
#else
    TEST_BOOL(tSimd::supports_AVX2() == false);
#endif
}

struct float4
{
    TMATH_SIMD_FLOAT4(float4)
};

struct Vector4f
{
    TMATH_VECTOR4(Vector4f, float)
};

void test()
{
    {
        {
            // load
            Vector4f v4f = { 1, 2, 3, 4.5 };
            float4 v = float4::load_from(v4f);
            TEST_BOOL(v.x == 1);
            TEST_BOOL(v.y == 2);
            TEST_BOOL(v.z == 3);
            TEST_BOOL(v.w == 4.5);
        }
        {
            // store
            float4 v = float4::load_from(Vector4f{ 1, 2, 3, 4 });
            Vector4f v4f{};
            v.store_to(v4f);
            TEST_BOOL(v4f.x == 1);
            TEST_BOOL(v4f.y == 2);
            TEST_BOOL(v4f.z == 3);
            TEST_BOOL(v4f.w == 4);
        }
    }
    {
        // operators
        {
            // +=
            float4 v(1, 2, 3, 4);
            float4 v2(-2, 3, -4, 5);
            v += v2;
            TEST_BOOL(v.x == -1);
            TEST_BOOL(v.y == 5);
            TEST_BOOL(v.z == -1);
            TEST_BOOL(v.w == 9);
        }
        {
            // -=
            float4 v(1, 2, 3, 4);
            float4 v2(-2, 3, -4, 5);
            v -= v2;
            TEST_BOOL(v.x == 3);
            TEST_BOOL(v.y == -1);
            TEST_BOOL(v.z == 7);
            TEST_BOOL(v.w == -1);
        }
    }
    {
        // abs
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