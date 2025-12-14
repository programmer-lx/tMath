#include <tMath/math.hpp>

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
    {
        // zero divide
        {
            int t = 0;
            float f = 10.0 / t;
            TEST_BOOL(tMath::is_infinity(f));
        }
        {
            int t = 0;
            float f = 0.0 / t;
            TEST_BOOL(tMath::is_nan(f));
        }
    }

    return 0;
}