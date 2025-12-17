#include <tMath/vector1.hpp>

#include "test.hpp"

int main()
{
    try
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
            float a = 10;
            TEST_BOOL(tMath::clamp(a, 0.0f, tMath::Epsilon<float>) == tMath::Epsilon<float>);
        }
        {
            TEST_BOOL(tMath::approximately(tMath::lerp(-10.0f, 10.0f, 0.6), 2.0f));
        }
        {
            // triangle functions
            float x = -0.1896513f;
            float y = 0.456987f;
            TEST_BOOL(tMath::approximately(tMath::sin(x), sin(x)));
            TEST_BOOL(tMath::approximately(tMath::asin(x), asin(x)));
            TEST_BOOL(tMath::approximately(tMath::cos(x), cos(x)));
            TEST_BOOL(tMath::approximately(tMath::acos(x), acos(x)));
            TEST_BOOL(tMath::approximately(tMath::tan(x), tan(x)));
            TEST_BOOL(tMath::approximately(tMath::atan(x), atan(x)));
            TEST_BOOL(tMath::approximately(tMath::atan2(y, x), atan2(y, x)));
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
            {
                float f = 1;
                float zero = 0;
                TEST_BOOL(tMath::safe_divide(f, zero, 100.0f) == 100.0f);
            }
            {
                float f = 1;
                float zero = 0;
                tMath::safe_divide_inplace(f, zero, 666.0f);
                TEST_BOOL(f == 666.0f);
            }
        }
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }

    std::cout << "SUCCEED (test_float)" << std::endl;

    return EXIT_SUCCESS;
}