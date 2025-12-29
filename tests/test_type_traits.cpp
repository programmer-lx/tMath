#include <tMath/math_defs.hpp>

#include "test.hpp"

TEST(NumberToFloatingPoint, IntegerTypes) {
    // 小整数 -> float
    EXPECT_TRUE((std::is_same_v<tMath::number_to_floating_point_t<int8_t>, float>));
    EXPECT_TRUE((std::is_same_v<tMath::number_to_floating_point_t<int16_t>, float>));

    // 大整数 -> double
    EXPECT_TRUE((std::is_same_v<tMath::number_to_floating_point_t<int32_t>, double>));
    EXPECT_TRUE((std::is_same_v<tMath::number_to_floating_point_t<int64_t>, double>));
}

TEST(NumberToFloatingPoint, FloatingPointTypes) {
    // float -> float
    EXPECT_TRUE((std::is_same_v<tMath::number_to_floating_point_t<float>, float>));

    // double -> double
    EXPECT_TRUE((std::is_same_v<tMath::number_to_floating_point_t<double>, double>));
}

TEST(NumberToFloatingPoint, MixedTypes) {
    // char -> float
    EXPECT_TRUE((std::is_same_v<tMath::number_to_floating_point_t<char>, float>));

    // short -> float
    EXPECT_TRUE((std::is_same_v<tMath::number_to_floating_point_t<short>, float>));

    // long -> double
    EXPECT_TRUE((std::is_same_v<tMath::number_to_floating_point_t<long>, double>));
}