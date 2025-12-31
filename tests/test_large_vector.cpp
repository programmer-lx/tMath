#include <tMath/vector.hpp>

#include "test.hpp"

template<typename T>
struct TestGenericVector2
{
    TMATH_FULL_VECTOR2(TestGenericVector2, T)
};

template<typename T, int D>
struct TestGenericVectorN
{
    TMATH_FULL_VECTOR_N(TestGenericVectorN, T, D)
};

TEST(vector2, generic)
{
    constexpr TestGenericVector2<float> v = { 1, 2 };
    constexpr TestGenericVector2<float> v2 = { 1, 2 };
    static_assert(tMath::approximately(v, v2));

    constexpr TestGenericVectorN<double, 3> v3 = { 1, 2, 3 };
    constexpr TestGenericVectorN<double, 4> v4 = { 1, 2, 3, 4 };
    static_assert(tMath::approximately(v3,
        tMath::vector_cast<TestGenericVectorN<double, 3>>(v4)
        ));

    SUCCEED();
}

TEST(magnitude_n, Int2D)
{
    auto r = tMath::magnitude(3, 4);
    static_assert(std::is_same_v<decltype(r), double>);

    EXPECT_DOUBLE_EQ(r, 5.0);
}

TEST(magnitude_n, Int3D)
{
    auto r = tMath::magnitude(1, 2, 2);
    EXPECT_DOUBLE_EQ(r, 3.0);
}

TEST(magnitude_n, Float3D)
{
    auto r = tMath::magnitude(1.0f, 2.0f, 2.0f);
    EXPECT_NEAR(r, 3.0f, 1e-6f);
}

TEST(magnitude_n, MixedTypes)
{
    auto r = tMath::magnitude(1, 2.0f, 2.0);
    static_assert(std::is_same_v<decltype(r), double>);

    EXPECT_NEAR(r, 3.0, 1e-9);
}

TEST(magnitude_n, Double4D)
{
    auto r = tMath::magnitude(1.0, 2.0, 3.0, 6.0);
    EXPECT_NEAR(r, std::sqrt(50.0), 1e-12);
}

TEST(DotTest, SmallVector) {
    // 测试小向量
    double res = tMath::dot(1.0, 2.0, 3.0, 4.0, 5.0, 6.0); // (1,2,3)·(4,5,6)
    EXPECT_DOUBLE_EQ(res, 32.0);
}

TEST(DotTest, LargeVector) {
    constexpr size_t N = 100;
    std::array<double, N> v1{}, v2{};

    for (size_t i = 0; i < N; ++i) {
        v1[i] = static_cast<double>(i);
        v2[i] = static_cast<double>(i+1);
    }

    // 调用 dot 函数，需要展开成参数包
    auto combined = [&]<std::size_t... I>(std::index_sequence<I...>){
        return tMath::dot(v1[I]..., v2[I]...);
    }(std::make_index_sequence<N>{});

    // 参考值
    double expected = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0.0);
    EXPECT_NEAR(combined, expected, 1e-12);
}