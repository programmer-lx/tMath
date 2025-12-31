#include <tMath/vector.hpp>

#include "test.hpp"

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
    tMath::safe_divide_inplace(v, 0.0f, Vector2f{ -1.0f, -1.0f });
    EXPECT_FLOAT_EQ(v.x, -1.0f);

    Vector2f v2 = { 10.0f, 20.0f };
    tMath::safe_divide_inplace(v2, 2.0f, Vector2f{ 0, 0 });
    EXPECT_FLOAT_EQ(v2.x, 5.0f);
}

TEST(safe_divide, normal) {
    Vector2f v = { 10.0f, 20.0f };
    Vector2f res = tMath::safe_divide(v, 0.0f, Vector2f{ -1.0f, -1.0f });
    EXPECT_FLOAT_EQ(res.x, -1.0f);
    EXPECT_FLOAT_EQ(v.x, 10.0f); // 确认原向量未改动
}

// =============================================
// CASTS
// =============================================

TEST(vector_cast, normal) {
    // 维度相等
    constexpr Vector3f v3 = { 1.0f, 2.0f, 3.0f };
    constexpr Vector3i32 v3i = tMath::vector_cast<Vector3i32>(v3);
    static_assert(v3i[0] == 1 && v3i[2] == 3);

    // 截断分支 (RetN < InN)
    constexpr Vector2f v2 = tMath::vector_cast<Vector2f>(v3);
    static_assert(v2[0] == 1.0f && v2[1] == 2.0f);

    // 填充分支 (RetN > InN)
    constexpr Vector4f v4 = tMath::vector_cast<Vector4f>(v2);
    static_assert(v4[0] == 1.0f && v4[3] == 0.0f);
}

// =============================================
// FUNCTIONS
// =============================================

TEST(to_degrees, normal) {
    constexpr Vector2f rad = { tMath::PI<float>, tMath::PI<float> / 2.0f };
    constexpr Vector2f deg = tMath::to_degrees(rad);
    EXPECT_NEAR(deg.x, 180.0f, 1e-5f);
    EXPECT_NEAR(deg.y, 90.0f, 1e-5f);
}

TEST(to_radians, normal) {
    constexpr Vector2f deg = { 180.0f, 90.0f };
    constexpr Vector2f rad = tMath::to_radians(deg);
    EXPECT_NEAR(rad.x, tMath::PI<float>, 1e-5f);
    EXPECT_NEAR(rad.y, tMath::PI<float> / 2.0f, 1e-5f);
}

TEST(min, normal) {
    constexpr Vector2f a = { 1.0f, 10.0f };
    constexpr Vector2f b = { 5.0f, 2.0f };
    constexpr Vector2f res = tMath::min(a, b);
    static_assert(res[0] == 1.0f && res[1] == 2.0f);
}

TEST(max, normal) {
    constexpr Vector2f a = { 1.0f, 10.0f };
    constexpr Vector2f b = { 5.0f, 2.0f };
    constexpr Vector2f res = tMath::max(a, b);
    static_assert(res[0] == 5.0f && res[1] == 10.0f);
}

TEST(abs, normal) {
    Vector2f v = { -5.0f, 3.0f };
    Vector2f res = tMath::abs(v);
    EXPECT_FLOAT_EQ(res.x, 5.0f);
    EXPECT_FLOAT_EQ(res.y, 3.0f);
}

TEST(cwise_mul, normal) {
    constexpr Vector2f a = { 2.0f, 3.0f };
    constexpr Vector2f b = { 4.0f, 5.0f };
    constexpr Vector2f res = tMath::cwise_mul(a, b);
    static_assert(res[0] == 8.0f && res[1] == 15.0f);
}

TEST(cwise_div, normal) {
    constexpr Vector2f a = { 8.0f, 15.0f };
    constexpr Vector2f b = { 2.0f, 3.0f };
    constexpr Vector2f res = tMath::cwise_div(a, b);
    static_assert(res[0] == 4.0f && res[1] == 5.0f);
}

TEST(sin, normal) {
    Vector2f v = { 0.0f, tMath::PI<float> / 2.0f };
    Vector2f res = tMath::sin(v);
    EXPECT_NEAR(res.x, 0.0f, 1e-7f);
    EXPECT_NEAR(res.y, 1.0f, 1e-7f);
}

TEST(cos, normal) {
    Vector2f v = { 0.0f, tMath::PI<float> };
    Vector2f res = tMath::cos(v);
    EXPECT_NEAR(res.x, 1.0f, 1e-7f);
    EXPECT_NEAR(res.y, -1.0f, 1e-7f);
}

TEST(tan, normal) {
    Vector2f v = { 0.0f, 0.0f };
    Vector2f res = tMath::tan(v);
    EXPECT_FLOAT_EQ(res.x, 0.0f);
}

TEST(dot_vector, normal) {
    constexpr Vector2f a = { 1.0f, 2.0f };
    constexpr Vector2f b = { 3.0f, 4.0f };
    static_assert(tMath::dot(a, b) == 11.0f);
}

TEST(dot_scalar_pack, normal) {
    constexpr double res = tMath::dot(1.0, 2.0, 3.0, 4.0); // (1,2) * (3,4)
    EXPECT_DOUBLE_EQ(res, 11.0);
}

TEST(approximately_all, normal)
{
    {
        Vector2f v1 = { 1.0f + tMath::Epsilon<float>, 2.0f + tMath::Epsilon<float> };
        Vector2f v2 = { 1.0f, 2.0f };
        EXPECT_TRUE(tMath::approximately_all(v1, v2, tMath::Epsilon<float> * 10));
    }

    {
        Vector2f v1 = { 1.0f, 2.0f + tMath::Epsilon<float> * 2 };
        Vector2f v2 = { 1.0f, 2.0f };
        EXPECT_FALSE(tMath::approximately_all(v1, v2));
    }
}

TEST(approximately_cwise, vec2) {
    Vector2f a = { 1.0f, 2.0f };
    Vector2f b = { 1.0f, 5.0f }; // X接近，Y不接近

    Vector2f res = tMath::approximately_cwise(a, b);

    // X 分量应该近似相等 (1.0f)
    EXPECT_NE(res.x, 0.0f);
    // Y 分量应该不相等 (0.0f)
    EXPECT_FLOAT_EQ(res.y, 0.0f);
}

TEST(approximately_cwise, vec3) {
    Vector3f v1 = { 10.0f, 20.0f, 30.0f };
    Vector3f v2 = { 10.1f, 20.0f, 29.9f };

    // 使用较大的阈值 0.2f
    Vector3f res = tMath::approximately_cwise(v1, v2, 0.2f);

    // 在 0.2 阈值下，所有分量都应该被视为近似相等
    EXPECT_NE(res.x, 0.0f);
    EXPECT_NE(res.y, 0.0f);
    EXPECT_NE(res.z, 0.0f);
}

TEST(approximately_cwise, vec4) {
    Vector4f a = { 0.0f, -1.0f, 100.0f, 0.5f };
    Vector4f b = { 0.0f, -1.001f, 100.0f, 0.501f };

    // 默认阈值下
    Vector4f res = tMath::approximately_cwise(a, b);

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

    Vector2f res = tMath::approximately_cwise(a, b, eps);

    EXPECT_NE(res.x, 0.0f); // 刚好等于阈值，应为 true
    EXPECT_EQ(res.y, 0.0f); // 超过阈值，应为 false
}

TEST(approximately_cwise, zero_tolerance) {
    // 3. 零误差测试：必须完全相等
    Vector2f a = { 0.0f, 1.0f };
    Vector2f b = { 0.0f, 1.00001f };

    Vector2f res = tMath::approximately_cwise(a, b, 0.0f);

    EXPECT_NE(res.x, 0.0f); // 0.0 == 0.0 始终为真
    EXPECT_EQ(res.y, 0.0f); // 哪怕差一点也是假
}

TEST(magnitude_vector, normal) {
    // N <= 3 (使用 std::hypot)
    Vector2f v2 = { 3.0f, 4.0f };
    EXPECT_FLOAT_EQ(tMath::magnitude(v2), 5.0f);

    // N > 3 (使用 sqrt 展开)
    Vector4f v4 = { 1.0f, 1.0f, 1.0f, 1.0f };
    EXPECT_FLOAT_EQ(tMath::magnitude(v4), 2.0f);
}

TEST(magnitude_scalar_pack, normal) {
    // 2D/3D hypot
    EXPECT_DOUBLE_EQ(tMath::magnitude(3, 4), 5.0);
    // 4D+ sqrt
    EXPECT_DOUBLE_EQ(tMath::magnitude(1, 1, 1, 1), 2.0);
}

TEST(normalize_inplace, normal) {
    Vector2f v = { 3.0f, 0.0f };
    tMath::normalize_inplace(v);
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
}

TEST(safe_normalize_inplace, normal) {
    Vector2f v = { 0.0f, 0.0f };
    Vector2f fallback = { 1.0f, 0.0f };
    tMath::safe_normalize_inplace(v, fallback);
    EXPECT_FLOAT_EQ(v.x, 1.0f);
}

TEST(normalized_fp, normal) {
    Vector2f v = { 0.0f, 5.0f };
    Vector2f res = tMath::normalized(v);
    EXPECT_FLOAT_EQ(res.y, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 5.0f); // 原值不变
}

TEST(normalized_sint, normal) {
    Vector2i32 vi = { 3, 4 };
    Vector2f res = tMath::normalized<Vector2f>(vi);
    EXPECT_FLOAT_EQ(tMath::magnitude(res), 1.0f);
}

TEST(safe_normalized_fp, normal) {
    Vector2f zero = { 0.0f, 0.0f };
    Vector2f res = tMath::safe_normalized(zero, Vector2f{ 0, 1 });
    EXPECT_FLOAT_EQ(res.y, 1.0f);
}

TEST(safe_normalized_sint, normal) {
    Vector2i32 zero = { 0, 0 };
    Vector2f res = tMath::safe_normalized<Vector2f>(zero, Vector2f{ 0, 1 });
    EXPECT_FLOAT_EQ(res.y, 1.0f);
}

TEST(distance_fp, normal) {
    Vector2f a = { 0.0f, 0.0f };
    Vector2f b = { 3.0f, 4.0f };
    EXPECT_FLOAT_EQ(tMath::distance(a, b), 5.0f);
}

TEST(distance_sint, normal) {
    Vector2i32 a = { 1, 1 };
    Vector2i32 b = { 4, 5 };
    double d = tMath::distance(a, b);
    EXPECT_DOUBLE_EQ(d, 5.0);
}

TEST(clamp, normal) {
    constexpr Vector2f v = { -5.0f, 15.0f };
    constexpr Vector2f min_v = { 0.0f, 0.0f };
    constexpr Vector2f max_v = { 10.0f, 10.0f };
    constexpr Vector2f res = tMath::clamp(v, min_v, max_v);
    static_assert(res[0] == 0.0f && res[1] == 10.0f);
}

TEST(lerp, normal) {
    constexpr Vector2f a = { 0.0f, 10.0f };
    constexpr Vector2f b = { 10.0f, 20.0f };
    constexpr Vector2f res = tMath::lerp(a, b, 0.5f);
    static_assert(res[0] == 5.0f && res[1] == 15.0f);
}
