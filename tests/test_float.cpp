#include <../include/tMath/math.hpp>

#include "test.hpp"

int main()
{
    {
        TEST_BOOL(tMath::approximately(tMath::to_radians(180.0f), tMath::PI<float>));
    }
    {
        TEST_BOOL(tMath::approximately(tMath::to_degrees(tMath::PI<float>), 180.0f));
    }
    {
        TEST_BOOL(tMath::approximately(tMath::abs(-1.111f), 1.111f));
    }
    {
        TEST_BOOL(tMath::abs(-1) == 1);
    }

    return 0;
}