#define TMATH_TEST_AVX2

#include "benchmark_all.hpp"

using namespace tSimd;

static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_F16C, use_FMA3, use_AVX2}, {}));

int main()
{
    try
    {
        benchmark_all();
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}