#include <tSimd/impl/platform.hpp>
#ifdef TSIMD_X86_ANY

#define TSIMD_TEST_INTRINSIC SSE2



#include <string>

#undef TSIMD_DISPATCH_THIS_FILE
#define TSIMD_DISPATCH_THIS_FILE "batch/dyn_dispatch_float/basic.cpp" // this file
#include <tSimd/dispatch_this_file.hpp> // auto dispatch (在tSimd/batch.hpp前面)
#include <tSimd/batch.hpp> // 一定要在 tSimd/dispatch_this_file.hpp 后面


#include "../test.hpp"

#pragma message("dispatch intrinsic: \"" TMATH_STR("" TSIMD_DYN_FUNC_ATTR) "\"")


namespace tsimd
{
    namespace TSIMD_DYN_INSTRUCTION
    {
        TSIMD_DYN_FUNC_ATTR void kernel_dyn_impl(const float* TMATH_RESTRICT arr, const size_t N, float* TMATH_RESTRICT out_result) noexcept
        {
            using op = TSIMD_CURRENT_OP(float);
            using batch_t = op::batch_t;
            constexpr size_t Step = op::Lanes;

            // 测试SimdOp后端
            EXPECT_TRUE(op::CurrentInstruction == SimdInstruction::SSE2);
            EXPECT_TRUE(op::BatchSize == 16);
            EXPECT_TRUE(op::ElementSize == 4);
            EXPECT_TRUE(op::Lanes == 4);
            EXPECT_TRUE(op::BatchAlignment == 16);

            std::string cur_intrinsic = TMATH_STR("" TSIMD_DYN_FUNC_ATTR);
#if defined(TMATH_COMPILER_MSVC)
            EXPECT_TRUE(cur_intrinsic == "\"\"");
#elif defined(TMATH_COMPILER_GCC) || defined(TMATH_COMPILER_CLANG)
            EXPECT_TRUE(cur_intrinsic == "\"\" __attribute__((target(\"sse2\")))");
#else
    #error "Unknown compiler."
#endif

            batch_t sum_n = op::zero();
            for (size_t i = 0; i < N; i += Step)
            {
                batch_t tmp = op::loadu(arr + i);
                sum_n = op::add(sum_n, tmp);
            }
            float sum = op::reduce_sum(sum_n);

            *out_result = sum;
        }
    }
}


#if TSIMD_ONCE

// export impl function
TSIMD_DYN_DISPATCH_FUNC(kernel_dyn_impl);

void kernel(const float* arr, const size_t N, float* out_result) noexcept
{
    TSIMD_DYN_CALL(kernel_dyn_impl)(arr, N, out_result);
}

TEST(dyn_dispatch, basic)
{
    float numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    float expected = 36.0f;

    float result = -1.0f;
    kernel(numbers, 8, &result);

    EXPECT_NEAR(result, expected, 1e-5f);
}

int main(int argc, char **argv)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
#else
int main(int argc, char **argv)
{
    return 0;
}
#endif