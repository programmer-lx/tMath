#define TMATH_TEST_F16C

#include "test_all.inl"

using namespace tSimd;

static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_F16C}, {use_AVX2, use_FMA3}));

int main()
{
    try
    {
        test_all();
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}