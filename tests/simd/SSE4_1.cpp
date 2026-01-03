#define TMATH_TEST_SSE4_1

#include "test_all.inl"

using namespace tsimd;

static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1}, {use_AVX, use_AVX2, use_FMA3, use_F16C}));

TEST(sse4_1, support)
{
    auto result = runtime_check_simd_intrinsics();
    EXPECT_TRUE(result.SSE4_1);
}