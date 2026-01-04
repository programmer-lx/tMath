#include <tMath/vector.hpp>

#include "../test.hpp"

TEST(builtin_types, vecN)
{
    constexpr tmath::Vector<float, 4> v1 = { 1, 2, 3, 4 };
    constexpr tmath::Vector<float, 4> v2 = { 5, 6, 7, 8 };
    constexpr tmath::Vector<float, 4> result = v1 + v2;

    EXPECT_TRUE(tmath::approximately_all(result, { 6, 8, 10, 12 }));
}

TEST(builtin_types, vec2)
{
    constexpr tmath::Vector2<float> v1 = { 1, 2 };
    constexpr tmath::Vector2<float> v2 = { 3, 4 };
    constexpr tmath::Vector2<float> result = v1 + v2;
    constexpr tmath::Vector2<float> test = { 4, 6 };

    static_assert(result == test);

    SUCCEED();
}

TEST(builtin_types, vec3)
{
    constexpr tmath::Vector3<float> v1 = { 1, 2, 3 };
    constexpr tmath::Vector3<float> v2 = { 3, 4, 5 };
    constexpr tmath::Vector3<float> result = v1 + v2;
    constexpr tmath::Vector3<float> test = { 4, 6, 8 };

    static_assert(result == test);

    SUCCEED();
}

TEST(builtin_types, vec4)
{
    constexpr tmath::Vector4<float> v1 = { 1, 2, 3, 4 };
    constexpr tmath::Vector4<float> v2 = { 3, 4, 5, 6 };
    constexpr tmath::Vector4<float> result = v1 + v2;
    constexpr tmath::Vector4<float> test = { 4, 6, 8, 10 };

    static_assert(result == test);

    SUCCEED();
}
