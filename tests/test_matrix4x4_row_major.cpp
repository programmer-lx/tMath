#include <tMath/matrix_row_major.hpp>

#include "test.hpp"

struct Mat4x4f_RM
{
    TMATH_FULL_MATRIX4X4_ROW_MAJOR(Mat4x4f_RM, float)
};

struct Mat4x4i32_RM
{
    TMATH_FULL_MATRIX4X4_ROW_MAJOR(Mat4x4i32_RM, int32_t)
};

struct Vec4f
{
    TMATH_FULL_VECTOR4(Vec4f, float)
};


TEST(mat_RM, fields)
{
    Mat4x4f_RM m = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0 ,0, 1, 0,
        0, 0, 0, 1
    };
}