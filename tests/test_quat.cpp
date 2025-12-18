#include <tMath/vector4.hpp>
#include <tMath/quat.hpp>

#include "test.hpp"

// struct Quat
// {
//     // TODO
//     float x, y, z, w;
//     TMATH_MARK_AS_QUAT
// };

struct Vector4f32
{
    TMATH_VECTOR4(Vector4f32, float)
};

TEST(quat, tag)
{
    EXPECT_TRUE(tMath::is_vector4_float<Vector4f32> == true);
}

TMATH_TEST_MAIN()
