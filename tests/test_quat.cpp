#include <tMath/vector4.hpp>

#include "test.hpp"

struct Quat
{
    float x, y, z, w;
    TMATH_MARK_AS_QUAT
};

struct Vector4f32
{
    float x, y, z, w;
};

int main()
{
    {
        // quat tag
        TEST_BOOL(tMath::is_vector4_f32<Vector4f32> == true);
        TEST_BOOL(tMath::is_vector4_f32<Quat> == false);
        TEST_BOOL(tMath::is_quat<Quat> == true);
    }

    return 0;
}