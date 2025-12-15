#include <tMath/vector2.hpp>

#include "test.hpp"

struct Vector2f
{
    TMATH_VECTOR2(Vector2f, float)
};

static_assert(tMath::is_vector2_float<Vector2f>);

int main()
{
    Vector2f a = {1, 1};
    Vector2f b = { 2, 2 };

    Vector2f c = a + b;
    TEST_BOOL(tMath::approximately(c, {3, 3}));

    return 0;
}