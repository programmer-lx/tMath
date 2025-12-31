#include <vector>
#include <climits> // 用于 CHAR_BIT
#include <cstring>

#include <tMath/number.hpp>

#include "test.hpp"

TEST(one_block, float_bits)
{
    float result = tMath::one_block<float>();

    // 1. 获取浮点数的字节大小
    size_t byte_size = sizeof(float);

    // 2. 将结果拷贝到容器中
    std::vector<unsigned char> bytes(byte_size);
    std::memcpy(bytes.data(), &result, byte_size);

    // 3. 逐字节、逐位验证
    for (size_t i = 0; i < byte_size; ++i) {
        unsigned char current_byte = bytes[i];

        // 验证该字节的 8 个位（CHAR_BIT 通常为 8）
        for (int bit = 0; bit < CHAR_BIT; ++bit) {
            // 构造掩码检查第 bit 位是否为 1
            bool is_one = (current_byte >> bit) & 1;

            // 使用 SCOPED_TRACE 可以帮助你在报错时定位是哪个字节、哪一位出了问题
            SCOPED_TRACE("Testing byte " + std::to_string(i) + ", bit " + std::to_string(bit));
            EXPECT_TRUE(is_one);
        }

        // 或者更简单地直接验证整个字节是否为 0xFF
        // EXPECT_EQ(current_byte, 0xFF);
    }
}

TEST(approximately, normal)
{
    EXPECT_TRUE(tMath::approximately(1.0000005, 1.000001f, 0.000001f));
    EXPECT_TRUE(tMath::approximately(1.0000005, 1.000001, 0.000001));
}

TEST(to_radians, normal)
{
    constexpr float result = tMath::to_radians(180.0f);
    EXPECT_TRUE(tMath::approximately(result, tMath::PI<float>));
}

TEST(factorial, normal)
{
    EXPECT_EQ(tMath::factorial(10), 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
    EXPECT_EQ(tMath::factorial(11), 11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1);
    EXPECT_EQ(tMath::factorial(-1), 1);
    EXPECT_EQ(tMath::factorial(0), 1);
}

TEST(degrees, normal)
{
    constexpr float result = tMath::to_degrees(tMath::PI<float>);
    EXPECT_TRUE(tMath::approximately(result, 180.0f));
}

TEST(exp, normal)
{
    EXPECT_EQ(tMath::exp(5.0), std::exp(5.0));
    EXPECT_EQ(tMath::exp(5.0f), std::exp(5.0f));
}

TEST(min, normal)
{
    EXPECT_EQ(tMath::min(1, 2), std::min(1, 2));
}

TEST(max, normal)
{
    EXPECT_EQ(tMath::max(1, 2), std::max(1, 2));
}

TEST(abs, normal)
{
    EXPECT_TRUE(tMath::approximately(tMath::abs(-1.111f), 1.111f));
    EXPECT_TRUE(tMath::abs(-1) == 1);
}

TEST(clamp, normal)
{
    constexpr float a = 10;
    constexpr float result = tMath::clamp(a, 0.0f, tMath::Epsilon<float>);
    EXPECT_TRUE(result == tMath::Epsilon<float>);
}

TEST(lerp, normal)
{
    constexpr float result = tMath::lerp(-10.0f, 10.0f, 0.6);
    EXPECT_TRUE(tMath::approximately(result, 2.0f));
}

TEST(lerp_saturated, normal)
{
    constexpr float r1 = tMath::lerp_saturated(-10.0f, 10.0f, 1.2f);
    EXPECT_TRUE(tMath::approximately(r1, 10.0f));

    constexpr float r2 = tMath::lerp_saturated(-10.0f, 10.0f, -2.f);
    EXPECT_TRUE(tMath::approximately(r2, -10.0f));
}

TEST(min_max, normal)
{
    {
        // min
        constexpr float result = tMath::min(1, 2);
        EXPECT_EQ(result, 1);
    }
    {
        // max
        constexpr float result = tMath::max(2, 3);
        EXPECT_EQ(result, 3);
    }
}

TEST(triangle_functions, normal)
{
    float x = -0.1896513f;
    float y = 0.456987f;
    EXPECT_TRUE(tMath::approximately(tMath::sin(x), std::sin(x)));
    EXPECT_TRUE(tMath::approximately(tMath::asin(x), std::asin(x)));
    EXPECT_TRUE(tMath::approximately(tMath::cos(x), std::cos(x)));
    EXPECT_TRUE(tMath::approximately(tMath::acos(x), std::acos(x)));
    EXPECT_TRUE(tMath::approximately(tMath::tan(x), std::tan(x)));
    EXPECT_TRUE(tMath::approximately(tMath::atan(x), std::atan(x)));
    EXPECT_TRUE(tMath::approximately(tMath::atan2(y, x), std::atan2(y, x)));
}

TEST(zero_divide, normal)
{
    // zero divide
    {
        int t = 0;
        float f = 10.0 / t;
        EXPECT_TRUE(tMath::is_infinity(f));
    }
    {
        int t = 0;
        float f = 0.0 / t;
        EXPECT_TRUE(tMath::is_nan(f));
    }
    {
        float f = 1;
        float zero = 0;
        EXPECT_TRUE(tMath::safe_divide(f, zero, 100.0f) == 100.0f);
    }
    {
        float f = 1;
        float zero = 0;
        tMath::safe_divide_inplace(f, zero, 666.0f);
        EXPECT_TRUE(f == 666.0f);
    }
}
