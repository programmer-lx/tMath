#include <tMath/number.hpp>

#include "../test.hpp"

constexpr int basic_test() noexcept
{
    TMATH_IF_CONSTEVAL
    {
        return 1;
    }

    return 2;
}

TEST(ct_vector1, basic_test)
{
    static_assert(basic_test() == 1);
    static_assert(basic_test() != 2);

    constexpr int c_result = basic_test();
    EXPECT_EQ(c_result, 1);

    int result = basic_test();
    EXPECT_EQ(result, 2);

    EXPECT_EQ(basic_test(), 2);
    EXPECT_NE(basic_test(), 1);
}

constinit int a = tMath::abs(-1);

TEST(ct_vector1, abs)
{
    static_assert(tMath::abs(-1) == 1);
    static_assert(tMath::abs(-1.0f) == 1.0f);
    static_assert(tMath::abs(-0.0) == 0.0);
    static_assert(tMath::abs(-0.0F) == 0.0F);
    static_assert(tMath::abs(-0.0L) == 0.0L);

    EXPECT_EQ(a, 1);
}
