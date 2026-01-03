#define TMATH_DISABLE_SVML
#define TMATH_TEST_AVX2

#include "test_all.inl"

using namespace tsimd;

#ifdef _MSC_VER
static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_F16C, use_FMA3, use_AVX2}, {}));
#else
static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_AVX2}, {use_FMA3, use_F16C, use_SVML}));
#endif

TEST(avx2, support)
{
    auto result = runtime_check_simd_intrinsics();
    EXPECT_TRUE(result.AVX2);
}
