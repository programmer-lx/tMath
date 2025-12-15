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

int main()
{
    try
    {
        {
            // quat tag
            TEST_BOOL(tMath::is_vector4_float<Vector4f32> == true);
            // TEST_BOOL(tMath::is_vector4_float<Quat> == false);
            // TEST_BOOL(tMath::is_quat<Quat> == true);
        }
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}