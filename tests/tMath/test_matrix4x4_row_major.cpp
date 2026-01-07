#include <tMath/matrix.hpp>
#include <tMath/vector.hpp>

#include "../test.hpp"

TMATH_DIAGNOSTICS_PUSH

#if defined(TMATH_COMPILER_CLANG)
TMATH_IGNORE_WARNING("-Wmissing-braces")
#endif

struct Vec4f
{
    TMATH_FULL_VECTOR4(Vec4f, float)
};

struct Vec4d
{
    TMATH_FULL_VECTOR4(Vec4d, double)
};

struct Vec4i32
{
    TMATH_FULL_VECTOR4(Vec4i32, int32_t)
};

struct Mat4x4f_RM
{
    Vec4f data[4];
    TMATH_MATRIX_OPERATORS(Mat4x4f_RM)
};

struct Mat4x4d_RM
{
    Vec4d data[4];
    TMATH_MATRIX_OPERATORS(Mat4x4d_RM)
};

struct Mat4x4i32_RM
{
    Vec4i32 data[4];
};

struct Vec3f
{
    TMATH_FULL_VECTOR3(Vec3f, float)
};

struct Mat3x3f_RM
{
    Vec3f data[3];
    TMATH_MATRIX_OPERATORS(Mat3x3f_RM)
};

template<tmath::is_matrix_any_major Mat>
constexpr void concept_test(const Mat& m) noexcept
{
    (void)m;
}

TEST(mat4_RM, concept_test)
{
    {
        constexpr Mat4x4f_RM m = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        concept_test(m);
    }
    {
        constexpr Mat4x4f_RM m = {
            Vec4f{1, 0, 0, 0},
            Vec4f{0, 1, 0, 0},
            Vec4f{0, 0, 1, 0},
            Vec4f{0, 0, 0, 1}
        };
        concept_test(m);
    }
}

TEST(mat4_RM, equal)
{
    constexpr Mat4x4f_RM m = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    constexpr Mat4x4f_RM test = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    EXPECT_TRUE(m == test);
}

TEST(mat4_RM, not_equal)
{
    constexpr Mat4x4f_RM m = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    constexpr Mat4x4f_RM test = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 12
    };

    EXPECT_TRUE(m != test);
}

TEST(mat4_RM, mat4_mul_vec4)
{
    constexpr Mat4x4f_RM m = {
        1, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 3, 0,
        0, 0, 0, 4
    };
    constexpr Vec4f v = { 2, 3, 4, 5 };
    constexpr Vec4f result = m * v;
    constexpr Vec4f test = {2, 6, 12, 20};

    EXPECT_TRUE(result == test);
}



// ---------------------------------------- mat * mat ----------------------------------------

// 1. 基础测试：单位阵相乘 (I * I = I)
TEST(mat4_RM, identity_multiplication)
{
    constexpr Mat4x4f_RM identity = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    constexpr Mat4x4f_RM result = identity * identity;

    // 验证结果是否仍为单位阵
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            EXPECT_FLOAT_EQ(result.data[i].data[j], identity.data[i].data[j]);
        }
    }
}

// 2. 编译期计算测试 (关键：验证 constexpr 是否生效)
TEST(mat4_RM, constexpr_validation)
{
    constexpr Mat4x4f_RM m1 = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 1, 2, 3,
        4, 5, 6, 7
    };
    constexpr Mat4x4f_RM m2 = {
        0, 1, 0, 1,
        1, 0, 1, 0,
        0, 1, 0, 1,
        1, 0, 1, 0
    };

    // 如果 operator* 逻辑有问题，编译期就会报错
    constexpr Mat4x4f_RM result = m1 * m2;

    // 手算验证第一行第一列：(1*0 + 2*1 + 3*0 + 4*1) = 6
    static_assert(result.data[0].data[0] == 6.0f, "Compile-time multiplication failed!");
    EXPECT_FLOAT_EQ(result.data[0].data[0], 6.0f);
}

// 3. 缩放矩阵与位移矩阵模拟测试
TEST(mat4_RM, scaling_and_values)
{
    // 只有对角线有值的缩放矩阵
    constexpr Mat4x4f_RM scale = {
        2, 0, 0, 0,
        0, 3, 0, 0,
        0, 0, 4, 0,
        0, 0, 0, 1
    };

    // 每一行全等的矩阵
    constexpr Mat4x4f_RM m = {
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4
    };

    constexpr Mat4x4f_RM result = scale * m;

    // 预期结果：第一行全为 2，第二行全为 6，第三行全为 12，第四行全为 4
    EXPECT_FLOAT_EQ(result.data[0].data[0], 2.0f);
    EXPECT_FLOAT_EQ(result.data[1].data[1], 6.0f);
    EXPECT_FLOAT_EQ(result.data[2].data[2], 12.0f);
    EXPECT_FLOAT_EQ(result.data[3].data[3], 4.0f);
}

// 4. 零矩阵测试
TEST(mat4_RM, zero_matrix)
{
    constexpr Mat4x4f_RM zero = {0}; // 假设支持初始化列表清零
    constexpr Mat4x4f_RM m = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    Mat4x4f_RM result = m * zero;

    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            EXPECT_FLOAT_EQ(result.data[i].data[j], 0.0f);
}

// 5. 综合算式测试 (A * B != B * A)
TEST(mat4_RM, non_commutative)
{
    Mat4x4f_RM A = {
        1, 2, 0, 0,
        3, 4, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    Mat4x4f_RM B = {
        5, 6, 0, 0,
        7, 8, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    Mat4x4f_RM AB = A * B;
    Mat4x4f_RM BA = B * A;

    // AB[0][0] = 1*5 + 2*7 = 19
    // BA[0][0] = 5*1 + 6*3 = 23
    EXPECT_NE(AB.data[0].data[0], BA.data[0].data[0]);
    EXPECT_FLOAT_EQ(AB.data[0].data[0], 19.0f);
    EXPECT_FLOAT_EQ(BA.data[0].data[0], 23.0f);
}


// ------------------------------------------- transpose -------------------------------------------
// 1. 基础功能测试：验证非对称矩阵的转置
TEST(mat4_RM, transpose_basic)
{
    // 定义一个每一项都不相等的矩阵，方便观察位置变换
    constexpr Mat4x4f_RM m = {
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f,  10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };

    constexpr Mat4x4f_RM t = tmath::transpose(m);

    // 验证对角线元素（i == j）保持不变
    EXPECT_FLOAT_EQ(t.data[0].data[0], 1.0f);
    EXPECT_FLOAT_EQ(t.data[1].data[1], 6.0f);
    EXPECT_FLOAT_EQ(t.data[2].data[2], 11.0f);
    EXPECT_FLOAT_EQ(t.data[3].data[3], 16.0f);

    // 验证非对角线元素镜像交换：t[i][j] == m[j][i]
    EXPECT_FLOAT_EQ(t.data[0].data[1], 5.0f); // 对应 m.data[1].data[0]
    EXPECT_FLOAT_EQ(t.data[1].data[0], 2.0f); // 对应 m.data[0].data[1]
    EXPECT_FLOAT_EQ(t.data[3].data[0], 4.0f); // 对应 m.data[0].data[3]
    EXPECT_FLOAT_EQ(t.data[0].data[3], 13.0f); // 对应 m.data[3].data[0]
}

// 2. 编译期验证：利用 static_assert 确保 constexpr 逻辑正确
TEST(mat4_RM, transpose_constexpr)
{
    constexpr Mat4x4f_RM m = {
        0, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    
    constexpr Mat4x4f_RM t = tmath::transpose(m);
    
    static_assert(t.data[1].data[0] == 1.0f, "Transpose compile-time error: element mismatch");
    static_assert(t.data[0].data[1] == 0.0f, "Transpose compile-time error: element mismatch");
    
    SUCCEED(); // 编译通过即代表逻辑基本正确
}

// 3. 数学性质测试：(A^T)^T == A
TEST(mat4_RM, transpose_involution)
{
    Mat4x4f_RM m = {
        1.2f, 3.4f, 5.6f, 7.8f,
        9.0f, 2.1f, 4.3f, 6.5f,
        8.7f, 0.9f, 1.1f, 2.2f,
        3.3f, 4.4f, 5.5f, 6.6f
    };

    auto t = tmath::transpose(m);
    auto tt = tmath::transpose(t);

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            EXPECT_FLOAT_EQ(tt.data[i].data[j], m.data[i].data[j]);
        }
    }
}

// 4. 特殊矩阵测试：单位阵与对称阵
TEST(mat4_RM, transpose_special_matrices)
{
    // 单位阵转置仍为单位阵
    constexpr Mat4x4f_RM identity = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    auto t_id = tmath::transpose(identity);
    EXPECT_TRUE(t_id == identity); // 假设你重载了 ==

    // 对称矩阵转置仍为自身
    constexpr Mat4x4f_RM symmetric = {
        1, 7, 3, 0,
        7, 4, -5, 2,
        3, -5, 6, 1,
        0, 2, 1, 9
    };
    auto t_sym = tmath::transpose(symmetric);
    
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            EXPECT_FLOAT_EQ(t_sym.data[i].data[j], symmetric.data[i].data[j]);
        }
    }
}

// -------------------------------------------------- cast ----------------------------------------------------
TEST(matrix_cast, double_to_float_same_layout)
{
    // 定义一个双精度矩阵
    Mat4x4d_RM m_double = {
        1.0, 2.0, 3.0, 4.0,
        5.0, 6.0, 7.0, 8.0,
        9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };

    // 转换为单精度矩阵
    auto m_float = tmath::matrix_cast<Mat4x4f_RM>(m_double);

    // 验证数据
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            EXPECT_FLOAT_EQ(static_cast<float>(m_double.data[i].data[j]), m_float.data[i].data[j]);
        }
    }
}
TEST(matrix_cast, constexpr_validation)
{
    constexpr Mat4x4d_RM m1 = {
        2.0, 0.0, 0.0, 0.0,
        0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0,
        0.0, 0.0, 0.0, 2.0
    };

    // 在编译期进行转换
    constexpr Mat4x4f_RM m2 = tmath::matrix_cast<Mat4x4f_RM>(m1);

    static_assert(m2.data[0].data[0] == 2.0f, "matrix_cast constexpr failed");
    static_assert(m2.data[3].data[3] == 2.0f, "matrix_cast constexpr failed");

    EXPECT_EQ(m2.data[0].data[0], 2.0f);
}
// 预期行为：编译失败，因为行列不匹配
// TEST(matrix_cast, static_assert_fail)
// {
//     Mat4x4f_RM m4x4;
//     // 假设你有 Mat3x3f_RM 类型
//     auto m3x3 = tmath::matrix_cast<Mat3x3f_RM>(m4x4);
// }

TEST(matrix_cast, zero_and_negative)
{
    Mat4x4f_RM m_src = {
        -1.5f, 0.0f,  2.5f,  0.0f,
         0.0f, -3.0f, 0.0f,  4.0f,
         5.0f, 0.0f,  -7.0f, 0.0f,
         0.0f, 8.5f,  0.0f,  -10.0f
    };

    // 即使是相同类型也可以 cast
    auto m_dst = tmath::matrix_cast<Mat4x4f_RM>(m_src);

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            EXPECT_FLOAT_EQ(m_src.data[i].data[j], m_dst.data[i].data[j]);
        }
    }
}



// -------------------------------------------- identity & scale ------------------------------------------------
// 1. 验证 identity() 函数
TEST(matrix_gen, identity_standard)
{
    // 显式指定类型为 Mat4x4f_RM
    constexpr auto id = tmath::identity<Mat4x4f_RM>();

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            if (i == j) {
                EXPECT_FLOAT_EQ(id.data[i].data[j], 1.0f);
            } else {
                EXPECT_FLOAT_EQ(id.data[i].data[j], 0.0f);
            }
        }
    }
}

// 2. 验证 scale(scalar) 生成对角阵
TEST(matrix_gen, scale_diagonal)
{
    constexpr float s = 3.14f;
    constexpr auto m = tmath::scale<Mat4x4f_RM>(s);

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            if (i == j) {
                // 对角线应等于 scalar
                EXPECT_FLOAT_EQ(m.data[i].data[j], s);
            } else {
                // 其余位置应为 0
                EXPECT_FLOAT_EQ(m.data[i].data[j], 0.0f);
            }
        }
    }
}

// 3. 编译期静态校验 (非常重要，确保无运行时开销)
TEST(matrix_gen, compile_time_const)
{
    // 如果这些函数不是真正的 constexpr，编译会报错
    constexpr auto id = tmath::identity<Mat4x4f_RM>();
    constexpr auto s_mat = tmath::scale<Mat4x4f_RM>(5);

    // 使用 static_assert 进行双重保障
    static_assert(id.data[0].data[0] == 1.0f, "Identity [0][0] should be 1.0");
    static_assert(id.data[0].data[1] == 0.0f, "Identity [0][1] should be 0.0");
    static_assert(s_mat.data[2].data[2] == 5.0f, "Scale [2][2] should be 5.0");
    static_assert(s_mat.data[3].data[0] == 0.0f, "Scale [3][0] should be 0.0");

    SUCCEED();
}

// 4. 不同数值类型测试 (如 double)
TEST(matrix_gen, type_consistency)
{
    constexpr double s_val = 2.5;
    auto m_double = tmath::scale<Mat4x4d_RM>(s_val);

    EXPECT_DOUBLE_EQ(m_double.data[0].data[0], 2.5);
    EXPECT_DOUBLE_EQ(m_double.data[1].data[0], 0.0);
}

TMATH_DIAGNOSTICS_POP
