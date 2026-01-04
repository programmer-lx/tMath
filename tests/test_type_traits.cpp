#include <../include/tMath/impl/math_defs.hpp>

#include "test.hpp"

TEST(NumberToFloatingPoint, IntegerTypes) {
    // 小整数 -> float
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<int8_t>, float>);
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<int16_t>, float>);

    // 大整数 -> double
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<int32_t>, double>);
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<int64_t>, double>);

    SUCCEED();
}

TEST(NumberToFloatingPoint, FloatingPointTypes) {
    // float -> float
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<float>, float>);

    // double -> double
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<double>, double>);
    
    // long double -> long double
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<long double>, long double>);

    SUCCEED();
}

TEST(NumberToFloatingPoint, MixedTypes) {
    // char -> float
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<char>, float>);

    // short -> float
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<short>, float>);

    // long -> double
    static_assert(std::is_same_v<tmath::number_to_floating_point_t<long>, double>);

    SUCCEED();
}