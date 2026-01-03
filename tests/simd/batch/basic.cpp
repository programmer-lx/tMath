#define TMATH_TEST_AVX2

#include <cstring>
#include <cstdlib>

#include <tSimd/types/float32_4.hpp>

#include "../../test.hpp"

using namespace tsimd;

#if defined(TMATH_SIMD_USE_AVX)

TEST(sum, speed_test)
{
    static constexpr size_t SumSize = 16 * 10000;
    static constexpr size_t SumByteSize = SumSize * sizeof(float32);

    // 建议用 32 或 64 字节对齐，以支持 AVX
    float32* numbers = static_cast<float32*>(my_aligned_alloc(SumByteSize, 32));

    // 1. 先准备好数据，全局只生成这一次
    for (int i = 0; i < SumSize; ++i) {
        numbers[i] = random_f(-1.0f, 1.0f); // 范围小一点精度更好控制
    }

    float32 scalar_sum_val = 0.0f;

    // scalar test
    {
        ScopeTimer timer("scalar sum");
        float32 tmp = 0.0f;
        for (int i = 0; i < SumSize; ++i) {
            tmp += numbers[i];
        }
        scalar_sum_val = tmp;
        std::cout << std::format("Scalar Result: {}", scalar_sum_val) << std::endl;
    }

    // sse test
    {
        ScopeTimer timer("sse sum");
        __m128 sum4 = _mm_setzero_ps();
        for (int i = 0; i < SumSize; i += 4) {
            sum4 = _mm_add_ps(sum4, _mm_load_ps(&numbers[i]));
        }
        alignas(16) float32 res[4];
        _mm_store_ps(res, sum4);
        float32 sse_sum = res[0] + res[1] + res[2] + res[3];
        std::cout << std::format("SSE Result: {}", sse_sum) << std::endl;
        EXPECT_NEAR(sse_sum, scalar_sum_val, 1e-2); // 允许微小误差
    }

    // sse3 test (hadd)
    {
        ScopeTimer timer("sse3 sum (hadd)");
        __m128 sum4 = _mm_setzero_ps();
        for (int i = 0; i < SumSize; i += 4) {
            sum4 = _mm_add_ps(sum4, _mm_load_ps(&numbers[i]));
        }

        // sum4 = [d, c, b, a]
        sum4 = _mm_hadd_ps(sum4, sum4); // sum4 = [c+d, a+b, c+d, a+b]
        sum4 = _mm_hadd_ps(sum4, sum4); // sum4 = [a+b+c+d, .........]
        float32 sse_sum = _mm_cvtss_f32(sum4);
        std::cout << std::format("SSE3 Result: {}", sse_sum) << std::endl;
        EXPECT_NEAR(sse_sum, scalar_sum_val, 1e-2); // 允许微小误差
    }

    // avx test
    {
        ScopeTimer timer("avx sum");
        __m256 sum8 = _mm256_setzero_ps();
        for (int i = 0; i < SumSize; i += 8) {
            // 注意：_mm256_load_ps 要求 32 字节对齐
            sum8 = _mm256_add_ps(sum8, _mm256_load_ps(&numbers[i]));
        }

        // 水平加法求和
        sum8 = _mm256_hadd_ps(sum8, sum8); // [67, 45, 67, 45, 23, 01, 23, 01]
        sum8 = _mm256_hadd_ps(sum8, sum8); // [4567, 4567, 4567, 4567, 0123, 0123, 0123, 0123]
        __m128 high = _mm256_extractf128_ps(sum8, 1);
        __m128 low = _mm256_castps256_ps128(sum8);
        __m128 combined = _mm_add_ps(low, high); // [01234567, ..............]

        float32 avx_sum = _mm_cvtss_f32(combined);

        std::cout << std::format("AVX Result: {}", avx_sum) << std::endl;
        EXPECT_NEAR(avx_sum, scalar_sum_val, 1e-2);
    }

    my_aligned_free(numbers);
}

#endif