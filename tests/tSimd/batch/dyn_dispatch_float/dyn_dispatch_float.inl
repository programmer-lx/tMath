#pragma once

#include <tSimd/batch.hpp>

#include "../../../test.hpp"

#ifndef TSIMD_IS_TESTING
#error "no test macro"
#endif

static bool g_dyn_selector_init = false;

static void init_dyn_selector_if_not()
{
    if (!g_dyn_selector_init)
    {
        tsimd::InstructionSelector::init();
    }
    g_dyn_selector_init = true;
}


TSIMD_DYN_DISPATCH_FUNC(, void, , test_mem_ops_impl, (float* dst, const float* src, float set_val, size_t n), ) {
    using op = tsimd::SimdOp<CurrentSimdInstruction, float>;
    constexpr size_t Step = op::Lanes;

    // 测试 zero & store
    for (size_t i = 0; i < n; i += Step) {
        op::storeu(dst + i, op::zero());
    }
    for (size_t i = 0; i < n; ++i) EXPECT_EQ(dst[i], 0.0f);

    // 测试 set & store
    for (size_t i = 0; i < n; i += Step) {
        op::storeu(dst + i, op::set(set_val));
    }
    for (size_t i = 0; i < n; ++i) EXPECT_EQ(dst[i], set_val);

    // 测试 loadu & store
    for (size_t i = 0; i < n; i += Step) {
        op::storeu(dst + i, op::loadu(src + i));
    }
    for (size_t i = 0; i < n; ++i) EXPECT_EQ(dst[i], src[i]);
}

TEST(SimdOp, MemoryAndConstants) {
    init_dyn_selector_if_not();
    float src[16], dst[16];
    for (int i = 0; i < 16; ++i) src[i] = static_cast<float>(i);
    // 传入 n=16，确保涵盖各种指令集的步长
    test_mem_ops_impl()(dst, src, 3.14f, 16);
}


TSIMD_DYN_DISPATCH_FUNC(, void, , test_add_sub_impl, (const float* a, const float* b, size_t n), ) {
    using op = tsimd::SimdOp<CurrentSimdInstruction, float>;
    constexpr size_t Step = op::Lanes;
    float temp[16];

    // 测试 Add 迭代
    for (size_t i = 0; i < n; i += Step) {
        op::storeu(temp + i, op::add(op::loadu(a + i), op::loadu(b + i)));
    }
    for (size_t i = 0; i < n; ++i) EXPECT_NEAR(temp[i], a[i] + b[i], 1e-5f);

    // 测试 Sub 迭代
    for (size_t i = 0; i < n; i += Step) {
        op::storeu(temp + i, op::sub(op::loadu(a + i), op::loadu(b + i)));
    }
    for (size_t i = 0; i < n; ++i) EXPECT_NEAR(temp[i], a[i] - b[i], 1e-5f);
}

TEST(SimdOp, AddSub) {
    init_dyn_selector_if_not();
    float a[16], b[16];
    for (int i = 0; i < 16; ++i) { a[i] = 10.0f + i; b[i] = 2.0f; }
    test_add_sub_impl()(a, b, 16);
}

TSIMD_DYN_DISPATCH_FUNC(, void, , test_mul_div_impl, (const float* a, const float* b, size_t n), ) {
    using op = tsimd::SimdOp<CurrentSimdInstruction, float>;
    constexpr size_t Step = op::Lanes;
    float temp[16];

    // 测试 Mul 迭代
    for (size_t i = 0; i < n; i += Step) {
        op::storeu(temp + i, op::mul(op::loadu(a + i), op::loadu(b + i)));
    }
    for (size_t i = 0; i < n; ++i) EXPECT_NEAR(temp[i], a[i] * b[i], 1e-5f);

    // 测试 Div 迭代
    for (size_t i = 0; i < n; i += Step) {
        op::storeu(temp + i, op::div(op::loadu(a + i), op::loadu(b + i)));
    }
    for (size_t i = 0; i < n; ++i) EXPECT_NEAR(temp[i], a[i] / b[i], 1e-5f);
}

TEST(SimdOp, MulDiv) {
    init_dyn_selector_if_not();
    float a[16], b[16];
    for (int i = 0; i < 16; ++i) { a[i] = static_cast<float>(i + 1); b[i] = 2.0f; }
    test_mul_div_impl()(a, b, 16);
}


TSIMD_DYN_DISPATCH_FUNC(, void, , test_sum_reduction_impl, (const float* a, size_t n), ) {
    using op = tsimd::SimdOp<CurrentSimdInstruction, float>;
    using batch_t = typename op::batch_t;
    constexpr size_t Step = op::Lanes;

    batch_t acc = op::zero();
    float expected = 0.0f;

    for (size_t i = 0; i < n; i += Step) {
        batch_t data = op::loadu(a + i);
        acc = op::add(acc, data);
    }

    for (size_t i = 0; i < n; ++i)
    {
        expected += a[i];
    }

    float actual = op::sum(acc);
    // 水平求和容易积累浮点误差，阈值稍微放宽一点
    EXPECT_NEAR(actual, expected, 1e-3f);
}

TEST(SimdOp, SumReduction) {
    init_dyn_selector_if_not();
    float a[16];
    for (int i = 0; i < 16; ++i) a[i] = static_cast<float>(i) * 0.1f;
    test_sum_reduction_impl()(a, 16);
}
