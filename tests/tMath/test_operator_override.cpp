#include <tMath/vector.hpp>

#include "../test.hpp"

TMATH_DIAGNOSTICS_PUSH

#if defined(TMATH_COMPILER_CLANG)
TMATH_IGNORE_WARNING("-Wmissing-braces")
#endif

struct Vector2f
{
    TMATH_FULL_VECTOR2(Vector2f, float)
};

static_assert(tmath::is_vector2_float<Vector2f>);

TEST(tMath, operator_override)
{
    Vector2f a = {1, 1};
    Vector2f b = { 2, 2 };
    Vector2f c = a + b;
    EXPECT_TRUE(tmath::approximately_all(c, {3, 3}));
}

TMATH_DIAGNOSTICS_POP
