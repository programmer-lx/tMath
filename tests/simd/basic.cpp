#define TMATH_TEST_AVX2

#include <cstring>
#include <cstdlib>
#include <type_traits>
#include <format>

#include <tSimd/impl/platform.hpp>

#include "../test.hpp"

#if defined(TSIMD_USE_AVX)

TEST(basic, sum_speed_test)
{
    static constexpr size_t SumSize = 16 * 10000;
    static constexpr size_t SumByteSize = SumSize * sizeof(float);

    // 建议用 32 或 64 字节对齐，以支持 AVX
    float* numbers = static_cast<float*>(my_aligned_alloc(SumByteSize, 32));

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
            sum4 = _mm_add_ps(sum4, _mm_load_ps(&numbers[i]));
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
            sum4 = _mm_add_ps(sum4, _mm_load_ps(&numbers[i]));
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
            sum8 = _mm256_add_ps(sum8, _mm256_load_ps(&numbers[i]));
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
    sse,
    avx
};

template<backend simd_backend>
struct simd_ops;

template<>
struct simd_ops<backend::sse>
{
    using f32 = __m128;
    static constexpr size_t lanes = 4;

    static inline f32 load(float* x) noexcept
    {
        return _mm_load_ps(x);
    }

    static inline void store(float* x, f32 v) noexcept
    {
        _mm_store_ps(x, v);
    }

    static inline f32 zero() noexcept
    {
        return _mm_setzero_ps();
    }

    static inline f32 add(f32 lhs, f32 rhs) noexcept
    {
        return _mm_add_ps(lhs, rhs);
    }

    static inline float reduce_sum(f32 v) noexcept
    {
        f32 v1 = _mm_hadd_ps(v, v);
        v1 = _mm_hadd_ps(v1, v1);
        return _mm_cvtss_f32(v1);
    }
};

template<>
struct simd_ops<backend::avx>
{
    using f32 = __m256;
    static constexpr size_t lanes = 8;

    static inline f32 load(float* x) noexcept
    {
        return _mm256_load_ps(x);
    }

    static inline void store(float* x, f32 v) noexcept
    {
        _mm256_store_ps(x, v);
    }

    static inline f32 zero() noexcept
    {
        return _mm256_setzero_ps();
    }

    static inline f32 add(f32 lhs, f32 rhs) noexcept
    {
        return _mm256_add_ps(lhs, rhs);
    }

    static inline float reduce_sum(f32 v) noexcept
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

static backend current_backend = backend::sse;

// 用8个数进行测试
template<backend simd_backend>
void kernel_impl(float* data, size_t size, float& out_sum) noexcept
{
    using ops = simd_ops<simd_backend>;

    using f32 = ops::f32;
    constexpr size_t N = ops::lanes;

    f32 sum_n = ops::zero();
    for (size_t i = 0; i < size; i += N)
    {
        f32 d1 = ops::load(data + i);
        sum_n = ops::add(sum_n, d1);
    }

    float sum = ops::reduce_sum(sum_n);
    float correct_sum = 0.0f;
    for (size_t i = 0; i < size; ++i)
    {
        correct_sum += data[i];
    }

    out_sum = sum;

    EXPECT_NEAR(sum, correct_sum, 1e-5f);
}

// in_expect_out_real: 外部传入一个期望值，该函数返回一个实际值，然后在测试的时候进行比较两个值是否相等
void kernel(float* data, size_t size, float& out_sum, backend& in_expect_out_real) noexcept
{
    switch (current_backend)
    {
        case backend::sse:
            kernel_impl<backend::sse>(data, size, out_sum);
            in_expect_out_real = backend::sse;
            break;

        case backend::avx:
            kernel_impl<backend::avx>(data, size, out_sum);
            in_expect_out_real = backend::avx;
            break;
    }
}

TEST(basic, dispatch_test)
{
    alignas(32) float test_data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    // sse
    {
        float sum = -1.0f;
        current_backend = backend::sse;
        backend expect_backend = backend::sse;
        kernel(test_data, 8, sum, current_backend);
        EXPECT_NEAR(sum, 36, 1e-5f);
        EXPECT_TRUE(current_backend == expect_backend);
    }

    // avx
    {
        float sum = -1.0f;
        current_backend = backend::avx;
        backend expect_backend = backend::avx;
        kernel(test_data, 8, sum, current_backend);
        EXPECT_NEAR(sum, 36, 1e-5f);
        EXPECT_TRUE(current_backend == expect_backend);
    }
}

#endif