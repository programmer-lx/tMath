#include <tMath/vector.hpp>

#include "../test.hpp"


// 辅助函数：判断向量所有分量是否为 NaN
#define ExpectVecNaN(v) \
{ \
    constexpr auto N = TMATH_NAMESPACE_NAME::vector_traits<decltype(v)>::component_count; \
    for (size_t i = 0; i < N; ++i) { \
        EXPECT_TRUE(std::isnan(v.data[i])) << "Component at index " << i << " is not NaN"; \
    } \
}

// 辅助函数：判断向量所有分量是否为 0
#define ExpectVecZero(v) \
{ \
    constexpr auto N = TMATH_NAMESPACE_NAME::vector_traits<decltype(v)>::component_count; \
    for (size_t i = 0; i < N; ++i) { \
        EXPECT_FLOAT_EQ(v.data[i], 0.0f) << "Component at index " << i << " is not Zero"; \
    } \
}

struct Vector2f
{
    TMATH_FULL_VECTOR2(Vector2f, float)
};

struct Vector2d
{
    TMATH_FULL_VECTOR2(Vector2d, double)
};

struct Vector2i16
{
    TMATH_FULL_VECTOR2(Vector2i16, int16_t)
};

struct Vector2i32
{
    TMATH_FULL_VECTOR2(Vector2i32, int32_t)
};

struct Vector3f
{
    TMATH_FULL_VECTOR3(Vector3f, float)
};

struct Vector3d
{
    TMATH_FULL_VECTOR3(Vector3d, double)
};

struct Vector3i32
{
    TMATH_FULL_VECTOR3(Vector3i32, int32_t)
};

struct Vector3i16
{
    TMATH_FULL_VECTOR3(Vector3i16, int16_t)
};

struct Vector4f
{
    TMATH_FULL_VECTOR4(Vector4f, float)
};

struct Vector4d
{
    TMATH_FULL_VECTOR4(Vector4d, double)
};

struct Vector4i16
{
    TMATH_FULL_VECTOR4(Vector4i16, int16_t)
};

struct Vector4i32
{
    TMATH_FULL_VECTOR4(Vector4i32, int32_t)
};

struct Vector5f
{
    TMATH_FULL_VECTOR_N(Vector5f, float, 5)
};

template<typename T>
struct TestGenericVector2
{
    TMATH_FULL_VECTOR2(TestGenericVector2, T)
};

template<typename T, int D>
struct TestGenericVectorN
{
    TMATH_FULL_VECTOR_N(TestGenericVectorN, T, D)
};


// =============================================================================
// OPERATORS
// =============================================================================

TEST(operator_equal, normal) {
    constexpr Vector2f v1 = { 1.1f, 2.2f };
    constexpr Vector2f v2 = { 1.1f, 2.2f };
    static_assert(v1 == v2);
}

TEST(operator_not_equal, normal) {
    constexpr Vector2f v1 = { 1.1f, 2.2f };
    constexpr Vector2f v2 = { 1.1f, 2.3f };
    static_assert(v1 != v2);
}

TEST(operator_add_assign, normal) {
    Vector2f v1 = { 1.0f, 2.0f };
    Vector2f v2 = { 0.5f, 1.5f };
    v1 += v2;
    EXPECT_FLOAT_EQ(v1.x, 1.5f);
    EXPECT_FLOAT_EQ(v1.y, 3.5f);
}

TEST(operator_add, normal) {
    constexpr Vector2f v1 = { 1.0f, 2.0f };
    constexpr Vector2f v2 = { 0.5f, 1.5f };
    constexpr Vector2f res = v1 + v2;
    static_assert(res[0] == 1.5f && res[1] == 3.5f);
}

TEST(operator_sub_assign, normal) {
    Vector2f v1 = { 2.0f, 4.0f };
    Vector2f v2 = { 0.5f, 1.5f };
    v1 -= v2;
    EXPECT_FLOAT_EQ(v1.x, 1.5f);
    EXPECT_FLOAT_EQ(v1.y, 2.5f);
}

TEST(operator_sub, normal) {
    constexpr Vector2f v1 = { 2.0f, 4.0f };
    constexpr Vector2f v2 = { 0.5f, 1.5f };
    constexpr Vector2f res = v1 - v2;
    static_assert(res[0] == 1.5f && res[1] == 2.5f);
}

TEST(operator_mul_assign, normal) {
    Vector2f v1 = { 2.0f, 3.0f };
    v1 *= 2.0f;
    EXPECT_FLOAT_EQ(v1.x, 4.0f);
    EXPECT_FLOAT_EQ(v1.y, 6.0f);
}

TEST(operator_mul, normal) {
    constexpr Vector2f v1 = { 2.0f, 3.0f };
    constexpr Vector2f res = v1 * 3.0f;
    static_assert(res[0] == 6.0f && res[1] == 9.0f);
}

TEST(operator_div_assign, normal) {
    Vector2f v1 = { 6.0f, 8.0f };
    v1 /= 2.0f;
    EXPECT_FLOAT_EQ(v1.x, 3.0f);
    EXPECT_FLOAT_EQ(v1.y, 4.0f);
}

TEST(operator_div, normal) {
    constexpr Vector2f v1 = { 6.0f, 8.0f };
    constexpr Vector2f res = v1 / 2.0f;
    static_assert(res[0] == 3.0f && res[1] == 4.0f);
}

TEST(safe_divide_inplace, normal) {
    Vector2f v = { 10.0f, 20.0f };
    tmath::safe_divide_inplace(v, 0.0f, Vector2f{ -1.0f, -1.0f });
    EXPECT_FLOAT_EQ(v.x, -1.0f);

    Vector2f v2 = { 10.0f, 20.0f };
    tmath::safe_divide_inplace(v2, 2.0f, Vector2f{ 0, 0 });
    EXPECT_FLOAT_EQ(v2.x, 5.0f);
}

TEST(safe_divide, normal) {
    Vector2f v = { 10.0f, 20.0f };
    Vector2f res = tmath::safe_divide(v, 0.0f, Vector2f{ -1.0f, -1.0f });
    EXPECT_FLOAT_EQ(res.x, -1.0f);
    EXPECT_FLOAT_EQ(v.x, 10.0f); // 确认原向量未改动
}

// =============================================
// CASTS
// =============================================

TEST(vector_cast, normal) {
    // 维度相等
    constexpr Vector3f v3 = { 1.0f, 2.0f, 3.0f };
    constexpr Vector3i32 v3i = tmath::vector_cast<Vector3i32>(v3);
    static_assert(v3i[0] == 1 && v3i[2] == 3);

    // 截断分支 (RetN < InN)
    constexpr Vector2f v2 = tmath::vector_cast<Vector2f>(v3);
    static_assert(v2[0] == 1.0f && v2[1] == 2.0f);

    // 填充分支 (RetN > InN)
    constexpr Vector4f v4 = tmath::vector_cast<Vector4f>(v2);
    static_assert(v4[0] == 1.0f && v4[3] == 0.0f);
}

// =============================================
// FUNCTIONS
// =============================================

TEST(to_degrees, normal) {
    constexpr Vector2f rad = { tmath::PI<float>, tmath::PI<float> / 2.0f };
    constexpr Vector2f deg = tmath::to_degrees(rad);
    EXPECT_NEAR(deg.x, 180.0f, 1e-5f);
    EXPECT_NEAR(deg.y, 90.0f, 1e-5f);
}

TEST(to_radians, normal) {
    constexpr Vector2f deg = { 180.0f, 90.0f };
    constexpr Vector2f rad = tmath::to_radians(deg);
    EXPECT_NEAR(rad.x, tmath::PI<float>, 1e-5f);
    EXPECT_NEAR(rad.y, tmath::PI<float> / 2.0f, 1e-5f);
}

TEST(min, normal) {
    constexpr Vector2f a = { 1.0f, 10.0f };
    constexpr Vector2f b = { 5.0f, 2.0f };
    constexpr Vector2f res = tmath::min(a, b);
    static_assert(res[0] == 1.0f && res[1] == 2.0f);
}

TEST(max, normal) {
    constexpr Vector2f a = { 1.0f, 10.0f };
    constexpr Vector2f b = { 5.0f, 2.0f };
    constexpr Vector2f res = tmath::max(a, b);
    static_assert(res[0] == 5.0f && res[1] == 10.0f);
}

TEST(abs, normal) {
    Vector2f v = { -5.0f, 3.0f };
    Vector2f res = tmath::abs(v);
    EXPECT_FLOAT_EQ(res.x, 5.0f);
    EXPECT_FLOAT_EQ(res.y, 3.0f);
}

TEST(cwise_mul, normal) {
    constexpr Vector2f a = { 2.0f, 3.0f };
    constexpr Vector2f b = { 4.0f, 5.0f };
    constexpr Vector2f res = tmath::cwise_mul(a, b);
    static_assert(res[0] == 8.0f && res[1] == 15.0f);
}

TEST(cwise_div, normal) {
    constexpr Vector2f a = { 8.0f, 15.0f };
    constexpr Vector2f b = { 2.0f, 3.0f };
    constexpr Vector2f res = tmath::cwise_div(a, b);
    static_assert(res[0] == 4.0f && res[1] == 5.0f);
}

TEST(sin, normal) {
    Vector2f v = { 0.0f, tmath::PI<float> / 2.0f };
    Vector2f res = tmath::sin(v);
    EXPECT_NEAR(res.x, 0.0f, 1e-7f);
    EXPECT_NEAR(res.y, 1.0f, 1e-7f);
}

TEST(cos, normal) {
    Vector2f v = { 0.0f, tmath::PI<float> };
    Vector2f res = tmath::cos(v);
    EXPECT_NEAR(res.x, 1.0f, 1e-7f);
    EXPECT_NEAR(res.y, -1.0f, 1e-7f);
}

TEST(tan, normal) {
    Vector2f v = { 0.0f, 0.0f };
    Vector2f res = tmath::tan(v);
    EXPECT_FLOAT_EQ(res.x, 0.0f);
}

TEST(dot_vector, normal) {
    constexpr Vector2f a = { 1.0f, 2.0f };
    constexpr Vector2f b = { 3.0f, 4.0f };
    static_assert(tmath::dot(a, b) == 11.0f);
}

TEST(dot_scalar_pack, normal) {
    constexpr double res = tmath::dot(1.0, 2.0, 3.0, 4.0); // (1,2) * (3,4)
    EXPECT_DOUBLE_EQ(res, 11.0);
}

TEST(approximately_all, normal)
{
    {
        Vector2f v1 = { 1.0f + tmath::Epsilon<float>, 2.0f + tmath::Epsilon<float> };
        Vector2f v2 = { 1.0f, 2.0f };
        EXPECT_TRUE(tmath::approximately_all(v1, v2, tmath::Epsilon<float> * 10));
    }

    {
        Vector2f v1 = { 1.0f, 2.0f + tmath::Epsilon<float> * 2 };
        Vector2f v2 = { 1.0f, 2.0f };
        EXPECT_FALSE(tmath::approximately_all(v1, v2));
    }
}

TEST(approximately_cwise, vec2) {
    Vector2f a = { 1.0f, 2.0f };
    Vector2f b = { 1.0f, 5.0f }; // X接近，Y不接近

    Vector2f res = tmath::approximately_cwise(a, b);

    // X 分量应该近似相等 (1.0f)
    EXPECT_NE(res.x, 0.0f);
    // Y 分量应该不相等 (0.0f)
    EXPECT_FLOAT_EQ(res.y, 0.0f);
}

TEST(approximately_cwise, vec3) {
    Vector3f v1 = { 10.0f, 20.0f, 30.0f };
    Vector3f v2 = { 10.1f, 20.0f, 29.9f };

    // 使用较大的阈值 0.2f
    Vector3f res = tmath::approximately_cwise(v1, v2, 0.2f);

    // 在 0.2 阈值下，所有分量都应该被视为近似相等
    EXPECT_NE(res.x, 0.0f);
    EXPECT_NE(res.y, 0.0f);
    EXPECT_NE(res.z, 0.0f);
}

TEST(approximately_cwise, vec4) {
    Vector4f a = { 0.0f, -1.0f, 100.0f, 0.5f };
    Vector4f b = { 0.0f, -1.001f, 100.0f, 0.501f };

    // 默认阈值下
    Vector4f res = tmath::approximately_cwise(a, b);

    EXPECT_NE(res.x, 0.0f); // 0.0 == 0.0
    EXPECT_EQ(res.y, 0.0f); // 差 0.001 > Epsilon
    EXPECT_NE(res.z, 0.0f); // 100.0 == 100.0
    EXPECT_EQ(res.w, 0.0f); // 差 0.001 > Epsilon
}

TEST(approximately_cwise, precision) {
    // 测试正好在阈值边界上的情况
    Vector2f a = { 0, 1.0f };
    float eps = 1e-4f;
    Vector2f b = { eps, 1.0f + eps * 2.0f };

    Vector2f res = tmath::approximately_cwise(a, b, eps);

    EXPECT_NE(res.x, 0.0f); // 刚好等于阈值，应为 true
    EXPECT_EQ(res.y, 0.0f); // 超过阈值，应为 false
}

TEST(approximately_cwise, zero_tolerance) {
    // 3. 零误差测试：必须完全相等
    Vector2f a = { 0.0f, 1.0f };
    Vector2f b = { 0.0f, 1.00001f };

    Vector2f res = tmath::approximately_cwise(a, b, 0.0f);

    EXPECT_NE(res.x, 0.0f); // 0.0 == 0.0 始终为真
    EXPECT_EQ(res.y, 0.0f); // 哪怕差一点也是假
}

TEST(magnitude_vector, normal) {
    // N <= 3 (使用 std::hypot)
    Vector2f v2 = { 3.0f, 4.0f };
    EXPECT_FLOAT_EQ(tmath::magnitude(v2), 5.0f);

    // N > 3 (使用 sqrt 展开)
    Vector4f v4 = { 1.0f, 1.0f, 1.0f, 1.0f };
    EXPECT_FLOAT_EQ(tmath::magnitude(v4), 2.0f);
}

TEST(magnitude_scalar_pack, normal) {
    EXPECT_DOUBLE_EQ(tmath::magnitude(3, 4), 5.0);
    EXPECT_DOUBLE_EQ(tmath::magnitude(1, 1, 1, 1), 2.0);
}

TEST(magnitude_scalar_pack, normal_and_edge_cases) {
    const float INF = std::numeric_limits<float>::infinity();
    const float NANV = std::numeric_limits<float>::quiet_NaN();
    const float MAXF = std::numeric_limits<float>::max();

    // ---------- 正常情况 ----------
    EXPECT_FLOAT_EQ(tmath::magnitude(3.0, 4.0), 5.0);
    EXPECT_FLOAT_EQ(tmath::magnitude(1.0, 1.0, 1.0, 1.0), 2.0);

    // ---------- 零向量 ----------
    EXPECT_FLOAT_EQ(tmath::magnitude(0.0, 0.0), 0.0);
    EXPECT_FLOAT_EQ(tmath::magnitude(0.0, 0.0, 0.0, 0.0), 0.0);

    // ---------- 大数安全 ----------
    // auto result = tmath::magnitude(-MAXF, MAXF, MAXF, -MAXF, MAXF, MAXF);
    // static_assert(std::is_same_v<float, decltype(result)>);
    // EXPECT_FLOAT_EQ(result, MAXF);
    // EXPECT_TRUE(tmath::is_finite(result));

    // ---------- 包含无限 ----------
    EXPECT_EQ(tmath::magnitude(INF, 1.0), INF);
    EXPECT_EQ(tmath::magnitude(1.0, INF), INF);
    EXPECT_EQ(tmath::magnitude(INF, INF), INF);

    // ---------- 包含 NaN ----------
    EXPECT_TRUE(std::isnan(tmath::magnitude(NANV, 1.0)));
    EXPECT_TRUE(std::isnan(tmath::magnitude(1.0, NANV)));
    EXPECT_TRUE(std::isnan(tmath::magnitude(NANV, NANV)));

    // ---------- 混合大数 + INF ----------
    EXPECT_EQ(tmath::magnitude(MAXF, INF), INF);

    // ---------- 混合大数 + NaN ----------
    EXPECT_TRUE(std::isnan(tmath::magnitude(MAXF, NANV)));
}

// --- 1. 普通向量 ---
TEST(normalize_inplace, normal_vector) {
    Vector2f v = {3.0f, 4.0f};
    tmath::normalize_inplace(v);
    EXPECT_FLOAT_EQ(v.x, 0.6f);
    EXPECT_FLOAT_EQ(v.y, 0.8f);
}

// --- 2. 零向量 ---
TEST(normalize_inplace, zero_vector) {
    Vector2f v = {0.0f, 0.0f};
    tmath::normalize_inplace(v);
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
}

// --- 3. 单个分量非零 ---
TEST(normalize_inplace, single_nonzero) {
    Vector2f v = {0.0f, 5.0f};
    tmath::normalize_inplace(v);
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 1.0f);
}

// --- 4. 负分量 ---
TEST(normalize_inplace, negative_vector) {
    Vector3f v = {-1.0f, -2.0f, -2.0f};
    tmath::normalize_inplace(v);
    float len = std::sqrt(1.0f + 4.0f + 4.0f);
    EXPECT_FLOAT_EQ(v.x, -1.0f / len);
    EXPECT_FLOAT_EQ(v.y, -2.0f / len);
    EXPECT_FLOAT_EQ(v.z, -2.0f / len);
}

// --- 5. 四维向量 ---
TEST(normalize_inplace, four_dim_vector) {
    Vector4f v = {1.0f, 2.0f, 3.0f, 4.0f};
    tmath::normalize_inplace(v);
    float len = std::sqrt(1.0f + 4.0f + 9.0f + 16.0f);
    EXPECT_FLOAT_EQ(v.x, 1.0f / len);
    EXPECT_FLOAT_EQ(v.y, 2.0f / len);
    EXPECT_FLOAT_EQ(v.z, 3.0f / len);
    EXPECT_FLOAT_EQ(v.w, 4.0f / len);
}

// --- 6. 含 NaN ---
TEST(normalize_inplace, contains_nan) {
    Vector2f v = {1.0f, std::numeric_limits<float>::quiet_NaN()};
    tmath::normalize_inplace(v);
    EXPECT_TRUE(std::isnan(v.x));
    EXPECT_TRUE(std::isnan(v.y));
}

// 1. Normal: 正常数值测试
TEST(scalar_normalized, normal_values) {
    Vector2f v = { 3.0f, 4.0f }; // mag = 5.0
    Vector2f res = tmath::normalized(v);

    EXPECT_NEAR(res.x, 0.6f, 1e-6f);
    EXPECT_NEAR(res.y, 0.8f, 1e-6f);
}

// 2. Zero: 0向量应返回 0向量
TEST(scalar_normalized, zero_vector) {
    Vector3f v_zero = { 0.0f, 0.0f, 0.0f };
    Vector3f res = tmath::normalized(v_zero);

    ExpectVecZero(res);
}

// 3. Big Num: 模长平方导致溢出为 Inf
TEST(scalar_normalized, big_num_overflow) {
    // 1e30 的平方和会超过 float 的最大范围，导致 magnitude 得到 inf
    float big = 1e30f;
    Vector2f v_big = { big, big };
    Vector2f res = tmath::normalized(v_big);

    // 逻辑要求：如果 mag == inf，返回全 NaN 向量
    ExpectVecNaN(res);
}

// 4. Inf: 输入直接包含 Inf 分量
TEST(scalar_normalized, input_contains_inf) {
    float inf = std::numeric_limits<float>::infinity();

    // 即使只有一个分量是 inf，也要返回全分量 NaN
    Vector3f v_inf = { inf, 1.0f, 0.5f };
    Vector3f res = tmath::normalized(v_inf);

    ExpectVecNaN(res);
}

// 5. NaN: 输入包含 NaN
TEST(scalar_normalized, input_contains_nan) {
    float nan = std::numeric_limits<float>::quiet_NaN();

    // 输入包含 NaN 时，mag 结果也是 NaN，inv_mag 为 NaN，最终传播 NaN
    Vector2f v_nan = { nan, 1.0f };
    Vector2f res = tmath::normalized(v_nan);

    ExpectVecNaN(res);
}

// 额外测试：Double 类型兼容性
TEST(scalar_normalized, double_precision) {
    Vector2d v = { 0.0, 10.0 };
    Vector2d res = tmath::normalized(v);

    EXPECT_DOUBLE_EQ(res.y, 1.0);
    EXPECT_DOUBLE_EQ(res.x, 0.0);
}

// --- 1. 普通整数向量 ---
TEST(normalized_sint, normal_vector) {
    Vector2i32 vi = {3, 4};
    Vector2f res = tmath::normalized<Vector2f>(vi);
    EXPECT_FLOAT_EQ(tmath::magnitude(res), 1.0f);
    EXPECT_FLOAT_EQ(res.x, 0.6f);
    EXPECT_FLOAT_EQ(res.y, 0.8f);
}

// --- 2. 零向量 ---
TEST(normalized_sint, zero_vector) {
    Vector2i32 vi = {0, 0};
    Vector2f res = tmath::normalized<Vector2f>(vi);
    EXPECT_FLOAT_EQ(res.x, 0.0f);
    EXPECT_FLOAT_EQ(res.y, 0.0f);
    EXPECT_FLOAT_EQ(tmath::magnitude(res), 0.0f);
}

// --- 3. 单个分量非零 ---
TEST(normalized_sint, single_nonzero) {
    Vector2i32 vi = {0, 5};
    Vector2f res = tmath::normalized<Vector2f>(vi);
    EXPECT_FLOAT_EQ(res.x, 0.0f);
    EXPECT_FLOAT_EQ(res.y, 1.0f);
    EXPECT_FLOAT_EQ(tmath::magnitude(res), 1.0f);
}

// --- 4. 负整数分量 ---
TEST(normalized_sint, negative_vector) {
    Vector3i32 vi = {-1, -2, -2};
    Vector3f res3d = tmath::normalized<Vector3f>(vi);

    float len2d = std::sqrt(1.0f + 4.0f);
    float len3d = std::sqrt(1.0f + 4.0f + 4.0f);

    EXPECT_FLOAT_EQ(res3d.x, -1.0f / len3d);
    EXPECT_FLOAT_EQ(res3d.y, -2.0f / len3d);
    EXPECT_FLOAT_EQ(res3d.z, -2.0f / len3d);

    EXPECT_FLOAT_EQ(tmath::magnitude(res3d), 1.0f);
}

TEST(distance_fp, normal) {
    Vector2f a = { 0.0f, 0.0f };
    Vector2f b = { 3.0f, 4.0f };
    EXPECT_FLOAT_EQ(tmath::distance(a, b), 5.0f);
}

TEST(distance_sint, normal) {
    Vector2i32 a = { 1, 1 };
    Vector2i32 b = { 4, 5 };
    double d = tmath::distance(a, b);
    EXPECT_DOUBLE_EQ(d, 5.0);
}

TEST(clamp, normal) {
    constexpr Vector2f v = { -5.0f, 15.0f };
    constexpr Vector2f min_v = { 0.0f, 0.0f };
    constexpr Vector2f max_v = { 10.0f, 10.0f };
    constexpr Vector2f res = tmath::clamp(v, min_v, max_v);
    static_assert(res[0] == 0.0f && res[1] == 10.0f);
}

TEST(lerp, normal) {
    constexpr Vector2f a = { 0.0f, 10.0f };
    constexpr Vector2f b = { 10.0f, 20.0f };
    constexpr Vector2f res = tmath::lerp(a, b, 0.5f);
    static_assert(res[0] == 5.0f && res[1] == 15.0f);
}
