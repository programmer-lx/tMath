#include <tMath/vector2.hpp>

#include "test.hpp"

struct Vector2f
{
    TMATH_FULL_VECTOR2(Vector2f, float)
};

static_assert(tMath::is_vector2_float<Vector2f>);

TEST(tMath, operator_override)
{
    Vector2f a = {1, 1};
    Vector2f b = { 2, 2 };
    Vector2f c = a + b;
    EXPECT_TRUE(tMath::approximately(c, {3, 3}));
}

