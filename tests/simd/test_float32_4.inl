#pragma once

#include <cstring>
#include <cstdint>
#include <cmath>
#include <type_traits>

#include "test_types.hpp"

#include "../test.hpp"


#define EXPECT_VEC4_NAN(v) \
    EXPECT_TRUE(std::isnan(tsimd::get_x(v))); \
    EXPECT_TRUE(std::isnan(tsimd::get_y(v))); \
    EXPECT_TRUE(std::isnan(tsimd::get_z(v))); \
    EXPECT_TRUE(std::isnan(tsimd::get_w(v)))

// 辅助宏：判断 float32_4 的四个分量是否全为 0
#define EXPECT_VEC4_ZERO(v) \
    EXPECT_FLOAT_EQ(tsimd::get_x(v), 0.0f); \
    EXPECT_FLOAT_EQ(tsimd::get_y(v), 0.0f); \
    EXPECT_FLOAT_EQ(tsimd::get_z(v), 0.0f); \
    EXPECT_FLOAT_EQ(tsimd::get_w(v), 0.0f)


using tsimd::float32_4;

// =============================================================================
// LOAD FUNCTIONS
// =============================================================================

TEST(simd_load_vec4, normal) {
    alignas(16) Vector4f v4_aligned = { 1.0f, 2.0f, 3.0f, 4.0f };
    float32_4 res_a = tsimd::load(v4_aligned);
    EXPECT_FLOAT_EQ(tsimd::get_x(res_a), 1.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res_a), 4.0f);

    Vector4f v4_unaligned = { 5.0f, 6.0f, 7.0f, 8.0f };
    float32_4 res_u = tsimd::load(v4_unaligned);
    EXPECT_FLOAT_EQ(tsimd::get_y(res_u), 6.0f);
}

TEST(simd_load_scalar, normal) {
    float32_4 res = tsimd::load(5.5f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 5.5f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 5.5f);
}

TEST(simd_load_explicit, normal) {
    float32_4 res = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 1.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 4.0f);
}

TEST(simd_load_point_vec3, normal) {
    Vector3f v3 = { 1.0f, 2.0f, 3.0f };
    float32_4 res = tsimd::load_point(v3);
    EXPECT_FLOAT_EQ(tsimd::get_z(res), 3.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 1.0f);
}

TEST(simd_load_point_vec2, normal) {
    Vector2f v2 = { 10.0f, 20.0f };
    float32_4 res = tsimd::load_point(v2);
    EXPECT_FLOAT_EQ(tsimd::get_y(res), 20.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 1.0f);
}

TEST(simd_load_vector_vec3, normal) {
    Vector3f v3 = { 1.0f, 2.0f, 3.0f };
    float32_4 res = tsimd::load_vector(v3);
    EXPECT_FLOAT_EQ(tsimd::get_z(res), 3.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 0.0f);
}

TEST(simd_load_vector_vec2, normal) {
    Vector2f v2 = { 10.0f, 20.0f };
    float32_4 res = tsimd::load_vector(v2);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 10.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 0.0f);
}

// =============================================================================
// STORE FUNCTIONS
// =============================================================================

TEST(simd_store_vec4, normal) {
    float32_4 v = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    alignas(16) Vector4f out;
    tsimd::store(out, v);
    EXPECT_FLOAT_EQ(out.x, 1.0f);
    EXPECT_FLOAT_EQ(out.w, 4.0f);
}

TEST(simd_store_vec3, normal) {
    float32_4 v = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    Vector3f out = { 0, 0, 0 };
    tsimd::store(out, v);
    EXPECT_FLOAT_EQ(out.x, 1.0f);
    EXPECT_FLOAT_EQ(out.z, 3.0f);
}

TEST(simd_store_vec2, normal) {
    float32_4 v = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    Vector2f out = { 0, 0 };
    tsimd::store(out, v);
    EXPECT_FLOAT_EQ(out.x, 1.0f);
    EXPECT_FLOAT_EQ(out.y, 2.0f);
}

// =============================================================================
// GETTERS
// =============================================================================

TEST(simd_get_x, normal) {
    float32_4 v = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tsimd::get_x(v), 1.0f);
}

TEST(simd_get_y, normal) {
    float32_4 v = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tsimd::get_y(v), 2.0f);
}

TEST(simd_get_z, normal) {
    float32_4 v = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tsimd::get_z(v), 3.0f);
}

TEST(simd_get_w, normal) {
    float32_4 v = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(v), 4.0f);
}

// =============================================================================
// ARITHMETIC
// =============================================================================

TEST(simd_add, normal) {
    float32_4 a = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tsimd::load(10.0f, 20.0f, 30.0f, 40.0f);
    float32_4 res = tsimd::add(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 11.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 44.0f);
}

TEST(simd_sub, normal) {
    float32_4 a = tsimd::load(10.0f, 10.0f, 10.0f, 10.0f);
    float32_4 b = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 res = tsimd::sub(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 9.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 6.0f);
}

TEST(simd_min, normal) {
    float32_4 a = tsimd::load(1.0f, 10.0f, -5.0f, 0.0f);
    float32_4 b = tsimd::load(5.0f, 2.0f, 0.0f, -1.0f);
    float32_4 res = tsimd::min(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 1.0f);
    EXPECT_FLOAT_EQ(tsimd::get_z(res), -5.0f);
}

TEST(simd_max, normal) {
    float32_4 a = tsimd::load(1.0f, 10.0f, -5.0f, 0.0f);
    float32_4 b = tsimd::load(5.0f, 2.0f, 0.0f, -1.0f);
    float32_4 res = tsimd::max(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 5.0f);
    EXPECT_FLOAT_EQ(tsimd::get_y(res), 10.0f);
}

TEST(simd_clamp, normal) {
    float32_4 v = tsimd::load(-10.0f, 5.0f, 20.0f, 0.0f);
    float32_4 lo = tsimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 hi = tsimd::load(10.0f, 10.0f, 10.0f, 10.0f);
    float32_4 res = tsimd::clamp(v, lo, hi);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 0.0f);
    EXPECT_FLOAT_EQ(tsimd::get_z(res), 10.0f);
}

TEST(simd_hadamard_mul, normal) {
    float32_4 a = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tsimd::load(2.0f, 2.0f, 2.0f, 2.0f);
    float32_4 res = tsimd::cwise_mul(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 2.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 8.0f);
}

TEST(simd_mul_scalar, normal) {
    float32_4 a = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 res = tsimd::mul(a, 10.0f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 10.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 40.0f);
}

TEST(simd_hadamard_div, normal) {
    float32_4 a = tsimd::load(10.0f, 20.0f, 30.0f, 40.0f);
    float32_4 b = tsimd::load(2.0f, 2.0f, 2.0f, 2.0f);
    float32_4 res = tsimd::cwise_div(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 5.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 20.0f);
}

TEST(simd_div_scalar, normal) {
    float32_4 a = tsimd::load(10.0f, 20.0f, 30.0f, 40.0f);
    float32_4 res = tsimd::div(a, 10.0f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 1.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 4.0f);
}

TEST(simd_mul_add, normal) {
    float32_4 a = tsimd::load(2.0f, 2.0f, 2.0f, 2.0f);
    float32_4 b = tsimd::load(3.0f, 3.0f, 3.0f, 3.0f);
    float32_4 c = tsimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    float32_4 res = tsimd::mul_add(a, b, c);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 7.0f);
}

// =============================================================================
// MATH FUNCTIONS
// =============================================================================

TEST(simd_abs, normal) {
    float32_4 v = tsimd::load(-1.0f, 2.0f, -3.0f, 4.0f);
    float32_4 res = tsimd::abs(v);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 1.0f);
    EXPECT_FLOAT_EQ(tsimd::get_z(res), 3.0f);
}

TEST(simd_sqrt, normal) {
    float32_4 v = tsimd::load(4.0f, 9.0f, 16.0f, 25.0f);
    float32_4 res = tsimd::sqrt(v);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 2.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 5.0f);
}

TEST(simd_sin, normal) {
    float32_4 v = tsimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 res = tsimd::sin(v);
    SUCCEED();
}

TEST(simd_cos, normal) {
    float32_4 v = tsimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 res = tsimd::cos(v);
    SUCCEED();
}

TEST(simd_dot2, normal) {
    float32_4 a = tsimd::load(1.0f, 2.0f, 0.0f, 0.0f);
    float32_4 b = tsimd::load(3.0f, 4.0f, 0.0f, 0.0f);
    float32_4 res = tsimd::dot2(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 11.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 11.0f);
}

TEST(simd_dot3, normal) {
    float32_4 a = tsimd::load(1.0f, 2.0f, 3.0f, 0.0f);
    float32_4 b = tsimd::load(4.0f, 5.0f, 6.0f, 0.0f);
    float32_4 res = tsimd::dot3(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 32.0f);
}

TEST(simd_dot4, normal) {
    float32_4 a = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tsimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    float32_4 res = tsimd::dot4(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 10.0f);
}

TEST(simd_cross2, normal) {
    float32_4 a = tsimd::load(1.0f, 0.0f, 0.0f, 0.0f);
    float32_4 b = tsimd::load(0.0f, 1.0f, 0.0f, 0.0f);
    float32_4 res = tsimd::cross2(a, b);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 1.0f);
}

TEST(simd_cross3, normal) {
    float32_4 a = tsimd::load(1.0f, 0.0f, 0.0f, 0.0f);
    float32_4 b = tsimd::load(0.0f, 1.0f, 0.0f, 0.0f);
    float32_4 res = tsimd::cross3(a, b); // TODO
    SUCCEED();
}

TEST(simd_lerp, normal) {
    // 基础数据准备
    float32_4 a = tsimd::load(10.0f, 20.0f, 30.0f, 40.0f);
    float32_4 b = tsimd::load(20.0f, 40.0f, 60.0f, 80.0f);

    // 1. 测试 t = 0.0 (应返回 a)
    float32_4 res_0 = tsimd::lerp(a, b, 0.0f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res_0), 10.0f);
    EXPECT_FLOAT_EQ(tsimd::get_y(res_0), 20.0f);
    EXPECT_FLOAT_EQ(tsimd::get_z(res_0), 30.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res_0), 40.0f);

    // 2. 测试 t = 1.0 (应返回 b)
    float32_4 res_1 = tsimd::lerp(a, b, 1.0f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res_1), 20.0f);
    EXPECT_FLOAT_EQ(tsimd::get_y(res_1), 40.0f);
    EXPECT_FLOAT_EQ(tsimd::get_z(res_1), 60.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res_1), 80.0f);

    // 3. 测试 t = 0.5 (中点插值)
    float32_4 res_mid = tsimd::lerp(a, b, 0.5f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res_mid), 15.0f);
    EXPECT_FLOAT_EQ(tsimd::get_y(res_mid), 30.0f);
    EXPECT_FLOAT_EQ(tsimd::get_z(res_mid), 45.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res_mid), 60.0f);

    // 4. 测试外插值 (t > 1.0)
    // t = 2.0 -> a + (b-a)*2 = 10 + 10*2 = 30
    float32_4 res_extra = tsimd::lerp(a, b, 2.0f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res_extra), 30.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res_extra), 120.0f);

    // 5. 测试负向外插值 (t < 0.0)
    // t = -1.0 -> a + (b-a)*(-1) = 10 - 10 = 0
    float32_4 res_neg = tsimd::lerp(a, b, -1.0f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res_neg), 0.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res_neg), 0.0f);
}

TEST(simd_lerp, precision) {
    // 测试极小步长的精度
    float32_4 a = tsimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 b = tsimd::load(1.0f, 1.0f, 1.0f, 1.0f);

    float t = 1e-6f;
    float32_4 res = tsimd::lerp(a, b, t);

    // 验证所有通道是否都正确应用了步长
    EXPECT_NEAR(tsimd::get_x(res), 1e-6f, 1e-9f);
    EXPECT_NEAR(tsimd::get_w(res), 1e-6f, 1e-9f);
}

TEST(simd_lerp, same_test) {
    // 测试起止点相同的情况
    float32_4 a = tsimd::load(5.0f, 5.0f, 5.0f, 5.0f);
    float32_4 b = tsimd::load(5.0f, 5.0f, 5.0f, 5.0f);

    float32_4 res = tsimd::lerp(a, b, 0.75f);
    EXPECT_FLOAT_EQ(tsimd::get_x(res), 5.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 5.0f);
}

TEST(simd_approximately_cwise, normal) {
    // 1. 常规测试：混合匹配
    // X: 完全相等 -> True
    // Y: 差异在阈值内 (0.05 < 0.1) -> True
    // Z: 差异恰好在阈值上 (0.1 == 0.1) -> True
    // W: 差异在阈值外 (0.2 > 0.1) -> False
    float32_4 a = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tsimd::load(1.0f, 2.05f, 3.1f, 4.2f);
    float tolerance = 0.1f;

    float32_4 res = tsimd::approximately_cwise(a, b, tolerance);

    EXPECT_NE(tsimd::get_x(res), 0);
    EXPECT_NE(tsimd::get_y(res), 0);
    EXPECT_NE(tsimd::get_z(res), 0);
    EXPECT_EQ(tsimd::get_w(res), 0);
}

TEST(simd_approximately_cwise, precision_edge) {
    // 2. 精度边界测试 (使用较小的 Epsilon)
    float eps = 1e-6f;
    float32_4 a = tsimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    // X: 略小于 eps
    // Y: 略大于 eps
    float32_4 b = tsimd::load(1.0f + 0.9e-6f, 1.0f + 1.1e-6f, 1.0f, 1.0f);

    float32_4 res = tsimd::approximately_cwise(a, b, eps);

    EXPECT_NE(tsimd::get_x(res), 0);
    EXPECT_EQ(tsimd::get_y(res), 0);
}

TEST(simd_approximately_cwise, all_true_test) {
    // 3. 测试全部满足条件的情况
    float32_4 a = tsimd::load(10.0f, -10.0f, 0.0f, 100.0f);
    float32_4 b = tsimd::load(10.001f, -10.001f, 0.001f, 99.999f);

    float32_4 res = tsimd::approximately_cwise(a, b, 0.01f);

    // 验证是否所有通道都是全 1
    // 在 SIMD 中这通常对应 _mm_movemask_ps 结果为 0xF
    EXPECT_NE(tsimd::get_x(res), 0);
    EXPECT_NE(tsimd::get_y(res), 0);
    EXPECT_NE(tsimd::get_z(res), 0);
    EXPECT_NE(tsimd::get_w(res), 0);
}

TEST(simd_approximately_cwise, zero_tolerance) {
    // 4. 测试零容忍度（退化为完全相等比较）
    float32_4 a = tsimd::load(1.0f, 2.0f, 3.0f, 4.0f);
    float32_4 b = tsimd::load(1.0f, 2.0f, 3.0f, 4.00001f);

    float32_4 res = tsimd::approximately_cwise(a, b, 0.0f);

    EXPECT_NE(tsimd::get_z(res), 0);
    EXPECT_EQ(tsimd::get_w(res), 0);
}

TEST(simd_approximately_cwise, negative_values) {
    // 5. 测试负数（验证 abs_diff 逻辑是否正确）
    float32_4 a = tsimd::load(-1.0f, -5.0f, -10.0f, -20.0f);
    float32_4 b = tsimd::load(-1.1f, -4.9f, -10.1f, -19.9f);

    // 差值的绝对值都是 0.1
    float32_4 res = tsimd::approximately_cwise(a, b, 0.11f);

    EXPECT_NE(tsimd::get_x(res), 0);
    EXPECT_NE(tsimd::get_y(res), 0);
    EXPECT_NE(tsimd::get_z(res), 0);
    EXPECT_NE(tsimd::get_w(res), 0);
}

TEST(simd_magnitude2, positive) {
    // 1. 测试标准输入：x=3, y=4, z和w设为干扰值
    // 模长应仅由 x, y 决定：sqrt(3^2 + 4^2) = 5
    float32_4 v = tsimd::load(3.0f, 4.0f, 10.0f, 20.0f);
    float32_4 res = tsimd::magnitude2(v);

    // 验证所有通道是否都等于 5.0
    EXPECT_NEAR(tsimd::get_x(res), 5.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_y(res), 5.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_z(res), 5.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_w(res), 5.0f, 1e-6f);
}

TEST(simd_magnitude2, negative_xy_test) {
    // 2. 测试负数分量：x=-6, y=-8
    float32_4 v = tsimd::load(-6.0f, -8.0f, 5.0f, 5.0f);
    float32_4 res = tsimd::magnitude2(v);

    // 结果应为 10.0
    EXPECT_NEAR(tsimd::get_x(res), 10.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_z(res), 10.0f, 1e-6f); // 检查广播是否成功
}

TEST(simd_magnitude2, approx_mask_check) {
    // 3. 按照你要求的格式，验证掩码
    float32_4 a = tsimd::load(1.0f, 1.0f, 9.9f, 9.9f); // sqrt(1^2+1^2) = 1.414...
    float32_4 res = tsimd::magnitude2(a);

    float val = 1.4142135f;
    float32_4 b = tsimd::load(val, val, val, val);

    // 使用你的 approximately_cwise 检查
    float32_4 mask = tsimd::approximately_cwise(res, b, 0.0001f);

    EXPECT_NE(tsimd::get_x(mask), 0);
    EXPECT_NE(tsimd::get_y(mask), 0);
    EXPECT_NE(tsimd::get_z(mask), 0);
    EXPECT_NE(tsimd::get_w(mask), 0);
}

TEST(simd_magnitude, magnitude3_basic) {
    // 1. 标准测试：(2, 3, 6) -> sqrt(4 + 9 + 36) = sqrt(49) = 7
    // 将 w 设为较大干扰值（如 100），验证其不被计入结果
    float32_4 v = tsimd::load(2.0f, 3.0f, 6.0f, 100.0f);
    float32_4 res = tsimd::magnitude3(v);

    // 验证所有通道是否都等于 7.0
    EXPECT_NEAR(tsimd::get_x(res), 7.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_y(res), 7.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_z(res), 7.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_w(res), 7.0f, 1e-6f);
}

TEST(simd_magnitude, magnitude3_negative) {
    // 2. 负数测试：(-1, -2, 2) -> sqrt(1 + 4 + 4) = 3
    float32_4 v = tsimd::load(-1.0f, -2.0f, 2.0f, 0.0f);
    float32_4 res = tsimd::magnitude3(v);

    EXPECT_NEAR(tsimd::get_x(res), 3.0f, 1e-6f);
}

TEST(simd_magnitude, magnitude4_basic) {
    // 1. 标准测试：(1, 1, 1, 1) -> sqrt(4) = 2
    float32_4 v = tsimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    float32_4 res = tsimd::magnitude4(v);

    EXPECT_NEAR(tsimd::get_x(res), 2.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_y(res), 2.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_z(res), 2.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_w(res), 2.0f, 1e-6f);
}

TEST(simd_magnitude, magnitude4_special) {
    // 2. 勾股数扩展测试：(1, 2, 4, 10) -> sqrt(1 + 4 + 16 + 100) = sqrt(121) = 11
    float32_4 v = tsimd::load(1.0f, 2.0f, 4.0f, 10.0f);
    float32_4 res = tsimd::magnitude4(v);

    EXPECT_NEAR(tsimd::get_x(res), 11.0f, 1e-6f);
}

// =========================================== normalize2 ===========================================
// 1. normal 正常数测试
TEST(simd_normalize2, normal_2d) {
    // 向量 (3.0, 4.0)，模长应为 5.0，归一化后为 (0.6, 0.8)
    // 注意：normalized2 只处理前两个分量，但 SIMD 通常会返回全分量结果
    float32_4 v = tsimd::load(3.0f, 4.0f, 0.0f, 0.0f);
    float32_4 res = tsimd::normalize2(v);

    EXPECT_NEAR(tsimd::get_x(res), 0.6f, 1e-6f);
    EXPECT_NEAR(tsimd::get_y(res), 0.8f, 1e-6f);
}

// 2. zero / epsilon 测试
TEST(simd_normalize2, zero_vector) {
    // 0向量：要求安全返回 0 向量
    float32_4 v_zero = tsimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 res_zero = tsimd::normalize2(v_zero);
    EXPECT_VEC4_ZERO(res_zero);

    // 极小向量
    float epsilon = tmath::Epsilon<float>;
    float32_4 v_eps = tsimd::load(epsilon, 0.0f, 0.0f, 0.0f);
    float32_4 res_eps = tsimd::normalize2(v_eps);
    // 这种情况 mag > 0，通常返回 (1, 0, 0, 0)
    EXPECT_NEAR(tsimd::get_x(res_eps), 1.0f, 1e-6f);
}

// 3. big_num 测试：模长平方溢出为 inf
TEST(simd_normalize2, big_num_overflow) {
    // 1e20^2 = 1e40，超过了 float 最大值 (~3.4e38)
    // 这会导致 len_sq 变为 Inf，从而触发你的 mag_correct 掩码
    float big = 1e30f;
    float32_4 v = tsimd::load(big, big, 0.0f, 0.0f);
    float32_4 res = tsimd::normalize2(v);

    // 根据逻辑：mag == inf，返回 QuietNaN 向量
    EXPECT_VEC4_NAN(res);
}

// 4. inf 测试：输入包含 inf
TEST(simd_normalize2, input_has_inf) {
    float inf = std::numeric_limits<float>::infinity();

    // 情况 A：分量中有 Inf，导致模长也是 Inf
    float32_4 v = tsimd::load(inf, 1.0f, 0.0f, 0.0f);
    float32_4 res = tsimd::normalize2(v);

    EXPECT_VEC4_NAN(res);
}

// 5. nan 测试：输入包含 nan
TEST(simd_normalize2, input_has_nan) {
    float nan = std::numeric_limits<float>::quiet_NaN();

    // 情况 A：分量中有 NaN
    float32_4 v = tsimd::load(nan, 1.0f, 0.0f, 0.0f);
    float32_4 res = tsimd::normalize2(v);

    // 逻辑：nan 会导致 mag 也是 nan，cmpneq(nan, inf) 为真，
    // 但 div_ps(nan, nan) 会传播 nan
    EXPECT_VEC4_NAN(res);
}

// =========================================== normalize3 ===========================================
// 1. normal 正常数测试
TEST(simd_normalize3, normal_3d) {
    // 向量 (1.0, 1.0, 1.0)，模长应为 sqrt(3) ≈ 1.7320508
    // 归一化后各分量应约为 0.57735026
    float32_4 v = tsimd::load(1.0f, 1.0f, 1.0f, 0.0f);
    float32_4 res = tsimd::normalize3(v);

    float expected = 1.0f / std::sqrt(3.0f);
    EXPECT_NEAR(tsimd::get_x(res), expected, 1e-6f);
    EXPECT_NEAR(tsimd::get_y(res), expected, 1e-6f);
    EXPECT_NEAR(tsimd::get_z(res), expected, 1e-6f);
}

// 2. zero / epsilon 测试
TEST(simd_normalize3, zero_vector_3d) {
    // 0向量：要求安全返回 0 向量
    float32_4 v_zero = tsimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 res_zero = tsimd::normalize3(v_zero);
    EXPECT_VEC4_ZERO(res_zero);

    // 极小向量测试
    float epsilon = tmath::Epsilon<float>;
    // (eps, eps, eps) 的模长为 sqrt(3)*eps，依然大于 0
    float32_4 v_eps = tsimd::load(epsilon, epsilon, epsilon, 0.0f);
    float32_4 res_eps = tsimd::normalize3(v_eps);

    float expected = 1.0f / std::sqrt(3.0f);
    EXPECT_NEAR(tsimd::get_x(res_eps), expected, 1e-6f);
    EXPECT_NEAR(tsimd::get_y(res_eps), expected, 1e-6f);
    EXPECT_NEAR(tsimd::get_z(res_eps), expected, 1e-6f);
}

// 3. big_num 测试：模长平方溢出为 inf
TEST(simd_normalize3, big_num_overflow_3d) {
    // 在 3D 中，三个大数平方相加更容易溢出
    float big = 2e30f;
    float32_4 v = tsimd::load(big, big, big, 0.0f);
    float32_4 res = tsimd::normalize3(v);

    // mag == inf -> 返回 QuietNaN 向量
    EXPECT_VEC4_NAN(res);
}

// 4. inf 测试：输入包含 inf
TEST(simd_normalize3, input_has_inf_3d) {
    float inf = std::numeric_limits<float>::infinity();

    // 只要有一个分量是 Inf，模长即为 Inf
    float32_4 v = tsimd::load(1.0f, inf, 1.0f, 0.0f);
    float32_4 res = tsimd::normalize3(v);

    EXPECT_VEC4_NAN(res);
}

// 5. nan 测试：输入包含 nan
TEST(simd_normalize3, input_has_nan_3d) {
    float nan = std::numeric_limits<float>::quiet_NaN();

    // Z 分量包含 NaN
    float32_4 v = tsimd::load(1.0f, 1.0f, nan, 0.0f);
    float32_4 res = tsimd::normalize3(v);

    EXPECT_VEC4_NAN(res);
}

// =========================================== normalize4 ===========================================
// 1. normal 正常数测试
TEST(simd_normalize4, normal_4d) {
    // 向量 (1.0, 1.0, 1.0, 1.0)，模长 = sqrt(1^2 + 1^2 + 1^2 + 1^2) = 2.0
    // 归一化结果应为 (0.5, 0.5, 0.5, 0.5)
    float32_4 v = tsimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    float32_4 res = tsimd::normalize4(v);

    EXPECT_FLOAT_EQ(tsimd::get_x(res), 0.5f);
    EXPECT_FLOAT_EQ(tsimd::get_y(res), 0.5f);
    EXPECT_FLOAT_EQ(tsimd::get_z(res), 0.5f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 0.5f);
}

// 2. zero / epsilon 测试
TEST(simd_normalize4, zero_vector_4d) {
    // 0向量：要求安全返回 0 向量
    float32_4 v_zero = tsimd::load(0.0f, 0.0f, 0.0f, 0.0f);
    float32_4 res_zero = tsimd::normalize4(v_zero);
    EXPECT_VEC4_ZERO(res_zero);

    // 极小向量测试：(eps, 0, 0, 0) -> (1, 0, 0, 0)
    float epsilon = tmath::Epsilon<float>;
    float32_4 v_eps = tsimd::load(0.0f, 0.0f, 0.0f, epsilon); // 测试 w 分量
    float32_4 res_eps = tsimd::normalize4(v_eps);

    EXPECT_NEAR(tsimd::get_w(res_eps), 1.0f, 1e-6f);
    EXPECT_NEAR(tsimd::get_x(res_eps), 0.0f, 1e-6f);
}

// 3. big_num 测试：模长平方溢出为 inf
TEST(simd_normalize4, big_num_overflow_4d) {
    // 四个大数平方相加。2e30^2 = 4e60 (Inf)
    float big = 2e30f;
    float32_4 v = tsimd::load(big, big, big, big);
    float32_4 res = tsimd::normalize4(v);

    // 根据逻辑：mag == inf -> 返回全分量 NaN 向量
    EXPECT_VEC4_NAN(res);
}

// 4. inf 测试：输入包含 inf
TEST(simd_normalize4, input_has_inf_4d) {
    float inf = std::numeric_limits<float>::infinity();

    // 只要有一个分量（如 w）是 Inf，结果应为全分量 NaN
    float32_4 v = tsimd::load(1.0f, 2.0f, 3.0f, inf);
    float32_4 res = tsimd::normalize4(v);

    EXPECT_VEC4_NAN(res);
}

// 5. nan 测试：输入包含 nan
TEST(simd_normalize4, input_has_nan_4d) {
    float nan = std::numeric_limits<float>::quiet_NaN();

    // w 分量包含 NaN
    float32_4 v = tsimd::load(1.0f, 1.0f, 1.0f, nan);
    float32_4 res = tsimd::normalize4(v);

    // 传播性测试：由于 mag 计算涉及 w，结果应为全分量 NaN
    EXPECT_VEC4_NAN(res);
}

// ==================================================== magnitudeN sq ====================================================
// 1. 2D 向量模长平方测试
TEST(simd_magnitude_sq, magnitude2_sq_basic) {
    // 输入 (3, 4, 7, 11)，只应计算 3^2 + 4^2 = 25
    float32_4 v = tsimd::load(3.0f, 4.0f, 7.0f, 11.0f);
    float32_4 res = tsimd::magnitude2_sq(v);

    EXPECT_FLOAT_EQ(tsimd::get_x(res), 25.0f);
    EXPECT_FLOAT_EQ(tsimd::get_y(res), 25.0f);
    EXPECT_FLOAT_EQ(tsimd::get_z(res), 25.0f);
    EXPECT_FLOAT_EQ(tsimd::get_w(res), 25.0f);
}

// 2. 3D 向量模长平方测试
TEST(simd_magnitude_sq, magnitude3_sq_basic) {
    // 输入 (1, 2, 3, 10)，只应计算 1^2 + 2^2 + 3^2 = 14
    float32_4 v = tsimd::load(1.0f, 2.0f, 3.0f, 10.0f);
    float32_4 res = tsimd::magnitude3_sq(v);

    EXPECT_FLOAT_EQ(tsimd::get_x(res), 14.0f);
}

// 3. 4D 向量模长平方测试
TEST(simd_magnitude_sq, magnitude4_sq_basic) {
    // 输入 (1, 1, 1, 1)，计算 1^2 + 1^2 + 1^2 + 1^2 = 4
    float32_4 v = tsimd::load(1.0f, 1.0f, 1.0f, 1.0f);
    float32_4 res = tsimd::magnitude4_sq(v);

    EXPECT_FLOAT_EQ(tsimd::get_x(res), 4.0f);
}

// 4. 边界值测试：溢出检查
TEST(simd_magnitude_sq, overflow_check) {
    // 使用足够大的数使其平方后溢出
    float big = 2e20f; // (2e20)^2 = 4e40 > FLT_MAX
    float32_4 v = tsimd::load(big, 0.0f, 0.0f, 0.0f);
    float32_4 res = tsimd::magnitude4_sq(v);

    EXPECT_TRUE(std::isinf(tsimd::get_x(res)));
}

// 5. 边界值测试：NaN 传播
TEST(simd_magnitude_sq, nan_propagation) {
    float nan = std::numeric_limits<float>::quiet_NaN();
    // 即使只有 z 是 NaN，magnitude3_sq 也应该返回 NaN
    float32_4 v = tsimd::load(1.0f, 1.0f, nan, 1.0f);
    float32_4 res = tsimd::magnitude3_sq(v);

    EXPECT_TRUE(std::isnan(tsimd::get_x(res)));
}

// ======================================================= is_nan =======================================================
// 1. 测试全正常数值 (No NaN)
TEST(simd_is_nan, all_normal_values) {
    float32_4 v = tsimd::load(1.0f, -2.5f, 0.0f, 1e10f);
    float32_4 mask = tsimd::is_nan(v);

    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_x(mask)), 0x00000000U);
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_y(mask)), 0x00000000U);
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_z(mask)), 0x00000000U);
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_w(mask)), 0x00000000U);
}

// 2. 测试全 NaN 数值
TEST(simd_is_nan, all_nan_values) {
    float nan = std::numeric_limits<float>::quiet_NaN();
    float32_4 v = tsimd::load(nan, nan, nan, nan);
    float32_4 mask = tsimd::is_nan(v);

    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_x(mask)), 0xFFFFFFFFU);
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_y(mask)), 0xFFFFFFFFU);
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_z(mask)), 0xFFFFFFFFU);
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_w(mask)), 0xFFFFFFFFU);
}

// 3. 测试混合分量 (一部分 NaN，一部分正常)
TEST(simd_is_nan, mixed_components) {
    float nan = std::numeric_limits<float>::quiet_NaN();
    // 只有 X 和 Z 是 NaN
    float32_4 v = tsimd::load(nan, 1.23f, nan, 0.0f);
    float32_4 mask = tsimd::is_nan(v);

    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_x(mask)), 0xFFFFFFFFU); // NaN
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_y(mask)), 0x00000000U); // Normal
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_z(mask)), 0xFFFFFFFFU); // NaN
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_w(mask)), 0x00000000U); // Normal
}

// 4. 边界值测试：Infinity 不应被视为 NaN
TEST(simd_is_nan, infinity_is_not_nan) {
    float inf = std::numeric_limits<float>::infinity();
    float neg_inf = -std::numeric_limits<float>::infinity();
    float nan = std::numeric_limits<float>::quiet_NaN();

    float32_4 v = tsimd::load(inf, neg_inf, 3.14f, nan);
    float32_4 mask = tsimd::is_nan(v);

    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_x(mask)), 0x00000000U); // +Inf
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_y(mask)), 0x00000000U); // -Inf
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_z(mask)), 0x00000000U); // Normal
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_w(mask)), 0xFFFFFFFFU); // NaN
}

// 5. 特殊 NaN 测试 (Signaling NaN)
TEST(simd_is_nan, signaling_nan) {
    float snan = std::numeric_limits<float>::signaling_NaN();
    float32_4 v = tsimd::load(snan, 0.0f, 0.0f, 0.0f);
    float32_4 mask = tsimd::is_nan(v);

    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_x(mask)), 0xFFFFFFFFU);
    EXPECT_EQ(std::bit_cast<uint32_t>(tsimd::get_y(mask)), 0x00000000U);
}
