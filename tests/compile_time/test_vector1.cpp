#include "../test.hpp"

#include <tMath/compile_time/vector1.hpp>

namespace ct = tMath::CompileTime;


TEST(ct_vector1, fmod)
{
    {
        constexpr long double r = ct::fmod(50.123l, 123.456l);
        EXPECT_DOUBLE_EQ(r, std::fmod(50.123l, 123.456l));
    }
    {
        constexpr long double r = ct::fmod(-50.123l, 123.456l);
        EXPECT_DOUBLE_EQ(r, std::fmod(-50.123l, 123.456l));
    }
    {
        constexpr long double r = ct::fmod(-50.123l, -123.456l);
        EXPECT_DOUBLE_EQ(r, std::fmod(-50.123l, -123.456l));
    }
    {
        constexpr long double r = ct::fmod(50.123l, -123.456l);
        EXPECT_DOUBLE_EQ(r, std::fmod(50.123l, -123.456l));
    }
}

TEST(ct_vector1, abs)
{
    {
        constexpr int x = ct::abs(-5);
        EXPECT_EQ(x, 5);
    }
    {
        constexpr float x = ct::abs(-0.0f);
        EXPECT_EQ(x, 0.0f);
    }
}