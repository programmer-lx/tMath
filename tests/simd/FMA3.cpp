#define TMATH_TEST_FMA3

#include "test_all.inl"
#ifdef TMATH_USE_FMA3
#endif
using namespace tsimd;

#ifdef _MSC_VER
static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_FMA3, use_F16C, use_AVX2}, { }));
#else
static_assert(test_simd_intrinsics({use_FMA3}, {use_AVX2, use_F16C}));
#endif

TEST(fma3, support)
{
    auto result = runtime_check_simd_intrinsics();
    EXPECT_TRUE(result.FMA3);
}