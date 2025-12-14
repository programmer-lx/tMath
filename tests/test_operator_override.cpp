#include <tMath/math.hpp>

#include "test.hpp"

struct Vector2f
{
    float x = 0, y = 0;
};
TMATH_REGISTER_VECTOR_TYPE(Vector2f)

static_assert(tMath::is_vector2_f32<Vector2f>);

int main()
{
    Vector2f a = {1, 1};
    Vector2f b = { 2, 2 };

    Vector2f c = a + b;
    TEST_BOOL(tMath::approximately(c, {3, 3}));

    return 0;
}