#include <tMath/large_vector.hpp>

#include "test.hpp"

TEST(large_vector, dot_n)
{
    constexpr float v1[5] = { 1, 2, 3, 4, 5 };
    constexpr float v2[5] = { 6, 7, 8, 9, 10 };
    constexpr float result = tMath::dot_n<5>(v1, v2);

    EXPECT_FLOAT_EQ(result, 6 + 14 + 24 + 36 + 50);
}