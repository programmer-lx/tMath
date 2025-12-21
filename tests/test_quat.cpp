#include <tMath/vector4.hpp>
#include <tMath/quat.hpp>

#include "test.hpp"

struct Quatf
{
    TMATH_QUAT(Quatf, float)
};

struct Vector4f32
{
    TMATH_VECTOR4(Vector4f32, float)
};

TEST(quat, tag)
{
    EXPECT_TRUE(tMath::is_vector4_float<Vector4f32> == true);
    EXPECT_TRUE(tMath::is_quat<Quatf> == true);
}
