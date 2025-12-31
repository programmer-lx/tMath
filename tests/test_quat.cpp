#include <tMath/quat.hpp>
#include <tMath/vector.hpp>

#include "test.hpp"

struct Quatf
{
    TMATH_FULL_QUAT(Quatf, float)
};

struct Vector4f32
{
    TMATH_FULL_VECTOR4(Vector4f32, float)
};

TEST(quat, tag)
{
    EXPECT_TRUE(tMath::is_vector4_float<Vector4f32> == true);
    EXPECT_TRUE(tMath::is_quat<Quatf> == true);
}

TEST(quat, identity)
{
    constexpr Quatf q = tMath::identity<Quatf>();
    EXPECT_EQ(q.x, 0);
    EXPECT_EQ(q.y, 0);
    EXPECT_EQ(q.z, 0);
    EXPECT_EQ(q.w, 1);

    EXPECT_EQ(q.data[0], 0);
    EXPECT_EQ(q.data[1], 0);
    EXPECT_EQ(q.data[2], 0);
    EXPECT_EQ(q.data[3], 1);

    EXPECT_EQ(q[0], 0);
    EXPECT_EQ(q[1], 0);
    EXPECT_EQ(q[2], 0);
    EXPECT_EQ(q[3], 1);
}