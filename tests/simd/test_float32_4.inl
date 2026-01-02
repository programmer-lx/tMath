#pragma once

#include <cstring>
#include <cstdint>

#include "test_types.hpp"

#include "../test.hpp"



using tSimd::float32_4;

// =============================================================================
// LOAD FUNCTIONS
// =============================================================================

TEST(simd_load_vec4, normal) {
    alignas(16) Vector4f v4_aligned = { 1.0f, 2.0f, 3.0f, 4.0f };
    float32_4 res_a = tSimd::load(v4_aligned);
    EXPECT_FLOAT_EQ(tSimd::get_x(res_a), 1.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res_a), 4.0f);

    Vector4f v4_unaligned = { 5.0f, 6.0f, 7.0f, 8.0f };
    float32_4 res_u = tSimd::load(v4_unaligned);
    EXPECT_FLOAT_EQ(tSimd::get_y(res_u), 6.0f);
}

TEST(simd_load_scalar, normal) {
    float32_4 res = tSimd::load(5.5f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 5.5f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 5.5f);
}

TEST(simd_load_explicit, normal) {
    float32_4 res = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 1.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 4.0f);
}

TEST(simd_load_point_vec3, normal) {
    Vector3f v3 = { 1.0f, 2.0f, 3.0f };
    float32_4 res = tSimd::load_point(v3);
    EXPECT_FLOAT_EQ(tSimd::get_z(res), 3.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 1.0f);
}

TEST(simd_load_point_vec2, normal) {
    Vector2f v2 = { 10.0f, 20.0f };
    float32_4 res = tSimd::load_point(v2);
    EXPECT_FLOAT_EQ(tSimd::get_y(res), 20.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 1.0f);
}

TEST(simd_load_vector_vec3, normal) {
    Vector3f v3 = { 1.0f, 2.0f, 3.0f };
    float32_4 res = tSimd::load_vector(v3);
    EXPECT_FLOAT_EQ(tSimd::get_z(res), 3.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 0.0f);
}

TEST(simd_load_vector_vec2, normal) {
    Vector2f v2 = { 10.0f, 20.0f };
    float32_4 res = tSimd::load_vector(v2);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 10.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 0.0f);
}

// =============================================================================
// STORE FUNCTIONS
// =============================================================================

TEST(simd_store_vec4, normal) {
    float32_4 v = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    alignas(16) Vector4f out;
    tSimd::store(out, v);
    EXPECT_FLOAT_EQ(out.x, 1.0f);
    EXPECT_FLOAT_EQ(out.w, 4.0f);
}

TEST(simd_store_vec3, normal) {
    float32_4 v = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    Vector3f out = { 0, 0, 0 };
    tSimd::store(out, v);
    EXPECT_FLOAT_EQ(out.x, 1.0f);
    EXPECT_FLOAT_EQ(out.z, 3.0f);
}

TEST(simd_store_vec2, normal) {
    float32_4 v = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    Vector2f out = { 0, 0 };
    tSimd::store(out, v);
    EXPECT_FLOAT_EQ(out.x, 1.0f);
    EXPECT_FLOAT_EQ(out.y, 2.0f);
}

// =============================================================================
// GETTERS
// =============================================================================

TEST(simd_get_x, normal) {
    float32_4 v = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tSimd::get_x(v), 1.0f);
}

TEST(simd_get_y, normal) {
    float32_4 v = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tSimd::get_y(v), 2.0f);
}

TEST(simd_get_z, normal) {
    float32_4 v = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tSimd::get_z(v), 3.0f);
}

TEST(simd_get_w, normal) {
    float32_4 v = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(v), 4.0f);
}

// =============================================================================
// ARITHMETIC
// =============================================================================

TEST(simd_add, normal) {
    float32_4 a = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tSimd::load(10.0f, 20.0f, 30.0f, 40.0f);
    float32_4 res = tSimd::add(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 11.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 44.0f);
}

TEST(simd_sub, normal) {
    float32_4 a = tSimd::load(10.0f, 10.0f, 10.0f, 10.0f);
    float32_4 b = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 res = tSimd::sub(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 9.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 6.0f);
}

TEST(simd_min, normal) {
    float32_4 a = tSimd::load(1.0f, 10.0f, -5.0f, 0.0f);
    float32_4 b = tSimd::load(5.0f, 2.0f, 0.0f, -1.0f);
    float32_4 res = tSimd::min(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 1.0f);
    EXPECT_FLOAT_EQ(tSimd::get_z(res), -5.0f);
}

TEST(simd_max, normal) {
    float32_4 a = tSimd::load(1.0f, 10.0f, -5.0f, 0.0f);
    float32_4 b = tSimd::load(5.0f, 2.0f, 0.0f, -1.0f);
    float32_4 res = tSimd::max(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 5.0f);
    EXPECT_FLOAT_EQ(tSimd::get_y(res), 10.0f);
}

TEST(simd_clamp, normal) {
    float32_4 v = tSimd::load(-10.0f, 5.0f, 20.0f, 0.0f);
    float32_4 lo = tSimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 hi = tSimd::load(10.0f, 10.0f, 10.0f, 10.0f);
    float32_4 res = tSimd::clamp(v, lo, hi);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 0.0f);
    EXPECT_FLOAT_EQ(tSimd::get_z(res), 10.0f);
}

TEST(simd_hadamard_mul, normal) {
    float32_4 a = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tSimd::load(2.0f, 2.0f, 2.0f, 2.0f);
    float32_4 res = tSimd::cwise_mul(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 2.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 8.0f);
}

TEST(simd_mul_scalar, normal) {
    float32_4 a = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 res = tSimd::mul(a, 10.0f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 10.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 40.0f);
}

TEST(simd_hadamard_div, normal) {
    float32_4 a = tSimd::load(10.0f, 20.0f, 30.0f, 40.0f);
    float32_4 b = tSimd::load(2.0f, 2.0f, 2.0f, 2.0f);
    float32_4 res = tSimd::cwise_div(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 5.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 20.0f);
}

TEST(simd_div_scalar, normal) {
    float32_4 a = tSimd::load(10.0f, 20.0f, 30.0f, 40.0f);
    float32_4 res = tSimd::div(a, 10.0f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 1.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 4.0f);
}

TEST(simd_mul_add, normal) {
    float32_4 a = tSimd::load(2.0f, 2.0f, 2.0f, 2.0f);
    float32_4 b = tSimd::load(3.0f, 3.0f, 3.0f, 3.0f);
    float32_4 c = tSimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    float32_4 res = tSimd::mul_add(a, b, c);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 7.0f);
}

// =============================================================================
// MATH FUNCTIONS
// =============================================================================

TEST(simd_abs, normal) {
    float32_4 v = tSimd::load(-1.0f, 2.0f, -3.0f, 4.0f);
    float32_4 res = tSimd::abs(v);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 1.0f);
    EXPECT_FLOAT_EQ(tSimd::get_z(res), 3.0f);
}

TEST(simd_sqrt, normal) {
    float32_4 v = tSimd::load(4.0f, 9.0f, 16.0f, 25.0f);
    float32_4 res = tSimd::sqrt(v);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 2.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 5.0f);
}

TEST(simd_sin, normal) {
    float32_4 v = tSimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 res = tSimd::sin(v);
    SUCCEED();
}

TEST(simd_cos, normal) {
    float32_4 v = tSimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 res = tSimd::cos(v);
    SUCCEED();
}

TEST(simd_dot2, normal) {
    float32_4 a = tSimd::load(1.0f, 2.0f, 0.0f, 0.0f);
    float32_4 b = tSimd::load(3.0f, 4.0f, 0.0f, 0.0f);
    float32_4 res = tSimd::dot2(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 11.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 11.0f);
}

TEST(simd_dot3, normal) {
    float32_4 a = tSimd::load(1.0f, 2.0f, 3.0f, 0.0f);
    float32_4 b = tSimd::load(4.0f, 5.0f, 6.0f, 0.0f);
    float32_4 res = tSimd::dot3(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 32.0f);
}

TEST(simd_dot4, normal) {
    float32_4 a = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tSimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    float32_4 res = tSimd::dot4(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 10.0f);
}

TEST(simd_cross2, normal) {
    float32_4 a = tSimd::load(1.0f, 0.0f, 0.0f, 0.0f);
    float32_4 b = tSimd::load(0.0f, 1.0f, 0.0f, 0.0f);
    float32_4 res = tSimd::cross2(a, b);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 1.0f);
}

TEST(simd_cross3, normal) {
    float32_4 a = tSimd::load(1.0f, 0.0f, 0.0f, 0.0f);
    float32_4 b = tSimd::load(0.0f, 1.0f, 0.0f, 0.0f);
    float32_4 res = tSimd::cross3(a, b); // TODO
    SUCCEED();
}

TEST(simd_lerp, normal) {
    // 基础数据准备
    float32_4 a = tSimd::load(10.0f, 20.0f, 30.0f, 40.0f);
    float32_4 b = tSimd::load(20.0f, 40.0f, 60.0f, 80.0f);

    // 1. 测试 t = 0.0 (应返回 a)
    float32_4 res_0 = tSimd::lerp(a, b, 0.0f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res_0), 10.0f);
    EXPECT_FLOAT_EQ(tSimd::get_y(res_0), 20.0f);
    EXPECT_FLOAT_EQ(tSimd::get_z(res_0), 30.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res_0), 40.0f);

    // 2. 测试 t = 1.0 (应返回 b)
    float32_4 res_1 = tSimd::lerp(a, b, 1.0f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res_1), 20.0f);
    EXPECT_FLOAT_EQ(tSimd::get_y(res_1), 40.0f);
    EXPECT_FLOAT_EQ(tSimd::get_z(res_1), 60.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res_1), 80.0f);

    // 3. 测试 t = 0.5 (中点插值)
    float32_4 res_mid = tSimd::lerp(a, b, 0.5f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res_mid), 15.0f);
    EXPECT_FLOAT_EQ(tSimd::get_y(res_mid), 30.0f);
    EXPECT_FLOAT_EQ(tSimd::get_z(res_mid), 45.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res_mid), 60.0f);

    // 4. 测试外插值 (t > 1.0)
    // t = 2.0 -> a + (b-a)*2 = 10 + 10*2 = 30
    float32_4 res_extra = tSimd::lerp(a, b, 2.0f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res_extra), 30.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res_extra), 120.0f);

    // 5. 测试负向外插值 (t < 0.0)
    // t = -1.0 -> a + (b-a)*(-1) = 10 - 10 = 0
    float32_4 res_neg = tSimd::lerp(a, b, -1.0f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res_neg), 0.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res_neg), 0.0f);
}

TEST(simd_lerp, precision) {
    // 测试极小步长的精度
    float32_4 a = tSimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 b = tSimd::load(1.0f, 1.0f, 1.0f, 1.0f);

    float t = 1e-6f;
    float32_4 res = tSimd::lerp(a, b, t);

    // 验证所有通道是否都正确应用了步长
    EXPECT_NEAR(tSimd::get_x(res), 1e-6f, 1e-9f);
    EXPECT_NEAR(tSimd::get_w(res), 1e-6f, 1e-9f);
}

TEST(simd_lerp, same_test) {
    // 测试起止点相同的情况
    float32_4 a = tSimd::load(5.0f, 5.0f, 5.0f, 5.0f);
    float32_4 b = tSimd::load(5.0f, 5.0f, 5.0f, 5.0f);

    float32_4 res = tSimd::lerp(a, b, 0.75f);
    EXPECT_FLOAT_EQ(tSimd::get_x(res), 5.0f);
    EXPECT_FLOAT_EQ(tSimd::get_w(res), 5.0f);
}

TEST(simd_approximately_cwise, normal) {
    // 1. 常规测试：混合匹配
    // X: 完全相等 -> True
    // Y: 差异在阈值内 (0.05 < 0.1) -> True
    // Z: 差异恰好在阈值上 (0.1 == 0.1) -> True
    // W: 差异在阈值外 (0.2 > 0.1) -> False
    float32_4 a = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tSimd::load(1.0f, 2.05f, 3.1f, 4.2f);
    float tolerance = 0.1f;

    float32_4 res = tSimd::approximately_cwise(a, b, tolerance);

    EXPECT_NE(tSimd::get_x(res), 0);
    EXPECT_NE(tSimd::get_y(res), 0);
    EXPECT_NE(tSimd::get_z(res), 0);
    EXPECT_EQ(tSimd::get_w(res), 0);
}

TEST(simd_approximately_cwise, precision_edge) {
    // 2. 精度边界测试 (使用较小的 Epsilon)
    float eps = 1e-6f;
    float32_4 a = tSimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    // X: 略小于 eps
    // Y: 略大于 eps
    float32_4 b = tSimd::load(1.0f + 0.9e-6f, 1.0f + 1.1e-6f, 1.0f, 1.0f);

    float32_4 res = tSimd::approximately_cwise(a, b, eps);

    EXPECT_NE(tSimd::get_x(res), 0);
    EXPECT_EQ(tSimd::get_y(res), 0);
}

TEST(simd_approximately_cwise, all_true_test) {
    // 3. 测试全部满足条件的情况
    float32_4 a = tSimd::load(10.0f, -10.0f, 0.0f, 100.0f);
    float32_4 b = tSimd::load(10.001f, -10.001f, 0.001f, 99.999f);

    float32_4 res = tSimd::approximately_cwise(a, b, 0.01f);

    // 验证是否所有通道都是全 1
    // 在 SIMD 中这通常对应 _mm_movemask_ps 结果为 0xF
    EXPECT_NE(tSimd::get_x(res), 0);
    EXPECT_NE(tSimd::get_y(res), 0);
    EXPECT_NE(tSimd::get_z(res), 0);
    EXPECT_NE(tSimd::get_w(res), 0);
}

TEST(simd_approximately_cwise, zero_tolerance) {
    // 4. 测试零容忍度（退化为完全相等比较）
    float32_4 a = tSimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tSimd::load(1.0f, 2.0f, 3.0f, 4.00001f);

    float32_4 res = tSimd::approximately_cwise(a, b, 0.0f);

    EXPECT_NE(tSimd::get_z(res), 0);
    EXPECT_EQ(tSimd::get_w(res), 0);
}

TEST(simd_approximately_cwise, negative_values) {
    // 5. 测试负数（验证 abs_diff 逻辑是否正确）
    float32_4 a = tSimd::load(-1.0f, -5.0f, -10.0f, -20.0f);
    float32_4 b = tSimd::load(-1.1f, -4.9f, -10.1f, -19.9f);

    // 差值的绝对值都是 0.1
    float32_4 res = tSimd::approximately_cwise(a, b, 0.11f);

    EXPECT_NE(tSimd::get_x(res), 0);
    EXPECT_NE(tSimd::get_y(res), 0);
    EXPECT_NE(tSimd::get_z(res), 0);
    EXPECT_NE(tSimd::get_w(res), 0);
}

TEST(simd_magnitude2, positive) {
    // 1. 测试标准输入：x=3, y=4, z和w设为干扰值
    // 模长应仅由 x, y 决定：sqrt(3^2 + 4^2) = 5
    float32_4 v = tSimd::load(3.0f, 4.0f, 10.0f, 20.0f);
    float32_4 res = tSimd::magnitude2(v);

    // 验证所有通道是否都等于 5.0
    EXPECT_NEAR(tSimd::get_x(res), 5.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_y(res), 5.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_z(res), 5.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_w(res), 5.0f, 1e-6f);
}

TEST(simd_magnitude2, negative_xy_test) {
    // 2. 测试负数分量：x=-6, y=-8
    float32_4 v = tSimd::load(-6.0f, -8.0f, 5.0f, 5.0f);
    float32_4 res = tSimd::magnitude2(v);

    // 结果应为 10.0
    EXPECT_NEAR(tSimd::get_x(res), 10.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_z(res), 10.0f, 1e-6f); // 检查广播是否成功
}

TEST(simd_magnitude2, approx_mask_check) {
    // 3. 按照你要求的格式，验证掩码
    float32_4 a = tSimd::load(1.0f, 1.0f, 9.9f, 9.9f); // sqrt(1^2+1^2) = 1.414...
    float32_4 res = tSimd::magnitude2(a);

    float val = 1.4142135f;
    float32_4 b = tSimd::load(val, val, val, val);

    // 使用你的 approximately_cwise 检查
    float32_4 mask = tSimd::approximately_cwise(res, b, 0.0001f);

    EXPECT_NE(tSimd::get_x(mask), 0);
    EXPECT_NE(tSimd::get_y(mask), 0);
    EXPECT_NE(tSimd::get_z(mask), 0);
    EXPECT_NE(tSimd::get_w(mask), 0);
}

TEST(simd_magnitude, magnitude3_basic) {
    // 1. 标准测试：(2, 3, 6) -> sqrt(4 + 9 + 36) = sqrt(49) = 7
    // 将 w 设为较大干扰值（如 100），验证其不被计入结果
    float32_4 v = tSimd::load(2.0f, 3.0f, 6.0f, 100.0f);
    float32_4 res = tSimd::magnitude3(v);

    // 验证所有通道是否都等于 7.0
    EXPECT_NEAR(tSimd::get_x(res), 7.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_y(res), 7.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_z(res), 7.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_w(res), 7.0f, 1e-6f);
}

TEST(simd_magnitude, magnitude3_negative) {
    // 2. 负数测试：(-1, -2, 2) -> sqrt(1 + 4 + 4) = 3
    float32_4 v = tSimd::load(-1.0f, -2.0f, 2.0f, 0.0f);
    float32_4 res = tSimd::magnitude3(v);

    EXPECT_NEAR(tSimd::get_x(res), 3.0f, 1e-6f);
}

TEST(simd_magnitude, magnitude4_basic) {
    // 1. 标准测试：(1, 1, 1, 1) -> sqrt(4) = 2
    float32_4 v = tSimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    float32_4 res = tSimd::magnitude4(v);

    EXPECT_NEAR(tSimd::get_x(res), 2.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_y(res), 2.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_z(res), 2.0f, 1e-6f);
    EXPECT_NEAR(tSimd::get_w(res), 2.0f, 1e-6f);
}

TEST(simd_magnitude, magnitude4_special) {
    // 2. 勾股数扩展测试：(1, 2, 4, 10) -> sqrt(1 + 4 + 16 + 100) = sqrt(121) = 11
    float32_4 v = tSimd::load(1.0f, 2.0f, 4.0f, 10.0f);
    float32_4 res = tSimd::magnitude4(v);

    EXPECT_NEAR(tSimd::get_x(res), 11.0f, 1e-6f);
}
