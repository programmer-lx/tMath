#include <tMath/vector.hpp>

#include "test.hpp"

TEST(vector, builtin_types)
{
    constexpr tMath::Vector<float, 4> v1 = { 1, 2, 3, 4 };
    constexpr tMath::Vector<float, 4> v2 = { 5, 6, 7, 8 };
    constexpr tMath::Vector<float, 4> result = v1 + v2;

    EXPECT_TRUE(tMath::approximately(result, { 6, 8, 10, 12 }));
}