#include <cassert>
#include <cmath>

#include <iostream>
#include <string>

#undef TSIMD_DISPATCH_THIS_FILE
#define TSIMD_DISPATCH_THIS_FILE "simd_batch_dyn_dispatch.cpp" // this file
#include <tSimd/dispatch_this_file.hpp> // auto dispatch (在tSimd/batch.hpp前面)
#include <tSimd/batch.hpp> // 一定要在 tSimd/dispatch_this_file.hpp 后面

#pragma message("dispatch target = " TMATH_STR("" TSIMD_DYN_FUNC_ATTR))

namespace tsimd
{
    namespace TSIMD_DYN_INSTRUCTION
    {
        TSIMD_DYN_FUNC_ATTR void kernel_dyn_impl(const float* TMATH_RESTRICT arr, const float* TMATH_RESTRICT arr2, const float* TMATH_RESTRICT arr3, const size_t N, float* TMATH_RESTRICT out_result) noexcept
        {
            using op = TSIMD_DYN_SIMD_OP(float);
            using batch_t = op::batch_t;
            constexpr size_t Step = op::Lanes;

            // 测试
            std::string cur_intrinsic = TMATH_STR("" TSIMD_DYN_FUNC_ATTR);
#if defined(TMATH_COMPILER_MSVC)
            assert(cur_intrinsic == "\"\"");
#elif defined(TMATH_COMPILER_GCC) || defined(TMATH_COMPILER_CLANG)
            assert(cur_intrinsic.contains("__attribute__((target("));
#else
    #error "Unknown compiler."
#endif

            size_t i = 0;
            for (; i + Step <= N; i += Step)
            {
                batch_t a = op::loadu(arr + i);
                batch_t b = op::loadu(arr2 + i);
                batch_t c = op::loadu(arr3 + i);

                batch_t tmp = op::mul_add(a, b, c);
                op::storeu(out_result + i, tmp);
            }
            for (; i < N; ++i)
            {
                out_result[i] = arr[i] * arr2[i] + arr3[i];
            }
        }
    }
}


#if TSIMD_ONCE

// export impl function
TSIMD_DYN_DISPATCH_FUNC(kernel_dyn_impl);

void kernel(const float* arr, const float* arr2, const float* arr3, const size_t N, float* out_result) noexcept
{
    TSIMD_DYN_CALL(kernel_dyn_impl)(arr, arr2, arr3, N, out_result);
}

int main()
{
    float numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    float numbers2[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9 };
    float numbers3[] = { 11, 12, 13, 14, 15, 16, 17, 18, 19 };

    float result[8] = {};
    kernel(numbers, numbers2, numbers3, 8, result);

    for (int i = 0; i < 8; ++i)
    {
        float expected = numbers[i] * numbers2[i] + numbers3[i];
        assert(std::abs(expected - result[i]) <= 1e-5f);
    }

    std::cout << "SUCCEED" << std::endl;

    return 0;
}
#endif