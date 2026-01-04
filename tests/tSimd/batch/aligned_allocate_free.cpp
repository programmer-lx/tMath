#define TSIMD_TEST_AVX

#include <vector>

#include <tSimd/aligned_allocate.hpp>
#include <tSimd/batch.hpp>

#include "../../test.hpp"


TEST(aligned_allocate, alignment_test)
{
    constexpr uintptr_t alignment = 32;

    for (int i = 1; i < 20; ++i)
    {
        float* mem = tsimd::aligned_allocate<float*>(i * sizeof(float) * 8, alignment);

        EXPECT_TRUE(reinterpret_cast<uintptr_t>(mem) % alignment == 0);

        tsimd::aligned_free(mem);
    }
}

TEST(aligned_allocate, std_vector)
{
    using Arr = std::vector<float, tsimd::AlignedAllocator<float>>;
    constexpr size_t size = 88;

    EXPECT_TRUE(tsimd::AlignedAllocator<float>::Alignment == 32);

    Arr numbers;
    for (int i = 0; i < size; ++i)
    {
        numbers.push_back(random_f(-2.0f, 2.0f));
    }

    float expected = 0.0f;
    for (int i = 0; i < size; ++i)
    {
        expected += numbers[i];
    }

    // simd test
    __m256 sum_8 = _mm256_setzero_ps();
    for (int i = 0; i < size; i += 8)
    {
        __m256 tmp = _mm256_load_ps(numbers.data() + i);
        sum_8 = _mm256_add_ps(sum_8, tmp);
    }

    // reduce sum
    using op = tsimd::SimdOp<tsimd::SimdInstruction::AVX, float>;
    float result = op::sum(sum_8);

    EXPECT_NEAR(result, expected, 1e-4f);
}

int main(int argc, char **argv)
{
    tsimd::InstructionSelector::init();
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}