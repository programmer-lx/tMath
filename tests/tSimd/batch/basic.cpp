#include <cstring>
#include <cstdlib>
#include <type_traits>
#include <format>
#include <array>

#define TSIMD_TEST_AVX

#include <tSimd/impl/platform.hpp>

#include "../../test.hpp"


TEST(basic, sum_speed_test)
{
    static constexpr size_t SumSize = 16 * 10000;
    static constexpr size_t SumByteSize = SumSize * sizeof(float);

    // float* numbers = static_cast<float*>(my_aligned_alloc(SumByteSize, 32));
    float* numbers = static_cast<float*>(my_aligned_alloc(SumByteSize, 4));

    // 1. 先准备好数据，全局只生成这一次
    for (int i = 0; i < SumSize; ++i) {
        numbers[i] = random_f(-1.0f, 1.0f); // 范围小一点精度更好控制
    }

    float scalar_sum_val = 0.0f;

    // scalar test
    {
        ScopeTimer timer("scalar sum");
        float tmp = 0.0f;
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
            sum4 = _mm_add_ps(sum4, _mm_loadu_ps(&numbers[i]));
        }
        alignas(16) float res[4];
        _mm_store_ps(res, sum4);
        float sse_sum = res[0] + res[1] + res[2] + res[3];
        std::cout << std::format("SSE Result: {}", sse_sum) << std::endl;
        EXPECT_NEAR(sse_sum, scalar_sum_val, 1e-2); // 允许微小误差
    }

    // sse3 test (hadd)
    {
        ScopeTimer timer("sse3 sum (hadd)");
        __m128 sum4 = _mm_setzero_ps();
        for (int i = 0; i < SumSize; i += 4) {
            sum4 = _mm_add_ps(sum4, _mm_loadu_ps(&numbers[i]));
        }

        // sum4 = [d, c, b, a]
        sum4 = _mm_hadd_ps(sum4, sum4); // sum4 = [c+d, a+b, c+d, a+b]
        sum4 = _mm_hadd_ps(sum4, sum4); // sum4 = [a+b+c+d, .........]
        float sse_sum = _mm_cvtss_f32(sum4);
        std::cout << std::format("SSE3 Result: {}", sse_sum) << std::endl;
        EXPECT_NEAR(sse_sum, scalar_sum_val, 1e-2); // 允许微小误差
    }

    // avx test
    {
        ScopeTimer timer("avx sum");
        __m256 sum8 = _mm256_setzero_ps();
        for (int i = 0; i < SumSize; i += 8) {
            // 注意：_mm256_load_ps 要求 32 字节对齐
            sum8 = _mm256_add_ps(sum8, _mm256_loadu_ps(&numbers[i]));
        }

        // 水平加法求和
        sum8 = _mm256_hadd_ps(sum8, sum8); // [67, 45, 67, 45, 23, 01, 23, 01]
        sum8 = _mm256_hadd_ps(sum8, sum8); // [4567, 4567, 4567, 4567, 0123, 0123, 0123, 0123]
        __m128 high = _mm256_extractf128_ps(sum8, 1);
        __m128 low = _mm256_castps256_ps128(sum8);
        __m128 combined = _mm_add_ps(low, high); // [01234567, ..............]

        float avx_sum = _mm_cvtss_f32(combined);

        std::cout << std::format("AVX Result: {}", avx_sum) << std::endl;
        EXPECT_NEAR(avx_sum, scalar_sum_val, 1e-2);
    }

    my_aligned_free(numbers);
}

enum class backend
{
    sse = 0,
    avx = 1,
    num
};

template<backend simd_backend = backend::num>
struct simd_ops
{
    static_assert(simd_backend != backend::num, "Please set a backend when using simd_ops<...>, for example: simd_ops<sse>, simd_ops<avx>, ...");
};

template<>
struct simd_ops<backend::sse>
{
    static constexpr backend current_backend = backend::sse; // 用于判断当前正在使用什么SIMD指令
    using f32 = __m128;
    static constexpr size_t lanes = 4;

    static inline f32 TSIMD_CALL_CONV load(float* x) noexcept
    {
        return _mm_loadu_ps(x);
    }

    static inline void TSIMD_CALL_CONV store(float* x, f32 v) noexcept
    {
        _mm_storeu_ps(x, v);
    }

    static inline f32 TSIMD_CALL_CONV zero() noexcept
    {
        return _mm_setzero_ps();
    }

    static inline f32 TSIMD_CALL_CONV add(f32 lhs, f32 rhs) noexcept
    {
        return _mm_add_ps(lhs, rhs);
    }

    static inline float TSIMD_CALL_CONV reduce_sum(f32 v) noexcept
    {
        f32 v1 = _mm_hadd_ps(v, v);
        v1 = _mm_hadd_ps(v1, v1);
        return _mm_cvtss_f32(v1);
    }
};

template<>
struct simd_ops<backend::avx>
{
    static constexpr backend current_backend = backend::avx;
    using f32 = __m256;
    static constexpr size_t lanes = 8;

    static inline f32 TSIMD_CALL_CONV load(float* x) noexcept
    {
        return _mm256_loadu_ps(x);
    }

    static inline void TSIMD_CALL_CONV store(float* x, f32 v) noexcept
    {
        _mm256_storeu_ps(x, v);
    }

    static inline f32 TSIMD_CALL_CONV zero() noexcept
    {
        return _mm256_setzero_ps();
    }

    static inline f32 TSIMD_CALL_CONV add(f32 lhs, f32 rhs) noexcept
    {
        return _mm256_add_ps(lhs, rhs);
    }

    static inline float TSIMD_CALL_CONV reduce_sum(f32 v) noexcept
    {
        // [1, 2, 3, 4, 5, 6, 7, 8]
        // [12, 34, 12, 34, 56, 78, 56, 78]
        // [1234, 1234, 1234, 1234, 5678, 5678, 5678, 5678]
        f32 v1 = _mm256_hadd_ps(v, v);
        v1 = _mm256_hadd_ps(v1, v1);

        __m128 low = _mm256_castps256_ps128(v1);
        __m128 high = _mm256_extractf128_ps(v1, 1);
        low = _mm_add_ps(low, high);
        return _mm_cvtss_f32(low);
    }
};

TEST(basic, isa_fn_correct)
{
    // sse
    {
        using ops = simd_ops<backend::sse>;

        alignas(16) float data[4] = { 1, 2, 3, 4 };
        ops::f32 v = ops::load(data);
        float sum = ops::reduce_sum(v);
        EXPECT_NEAR(sum, 10, 1e-5f);
    }
    // avx
    {
        using ops = simd_ops<backend::avx>;

        alignas(32) float data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        ops::f32 v = ops::load(data);
        float sum = ops::reduce_sum(v);
        EXPECT_NEAR(sum, 36, 1e-5f);
    }
}


int main(int argc, char **argv)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}