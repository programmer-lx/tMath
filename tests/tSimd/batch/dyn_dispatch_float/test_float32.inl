#include "../test.hpp"

// #define TSIMD_ONCE 1

// ------------------------------------------ zero ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_zero_impl(float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;

        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        for (size_t i = 0; i < TOTAL; i += Step)
        {
            op::storeu(out + i, op::zero());
        }
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_zero_impl);

static void kernel_zero(float* out) noexcept
{
    TSIMD_DYN_CALL(kernel_zero_impl)(out);
}

TEST(dyn_dispatch_x86_float32, zero)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;

    alignas(ALIGNMENT) float out[TOTAL];

    // 先填充非零值
    for (int i = 0; i < TOTAL; ++i) out[i] = -1.0f;

    kernel_zero(out);

    // 检查结果是否全为 0
    for (int i = 0; i < TOTAL; ++i)
        EXPECT_FLOAT_EQ(out[i], 0.0f);
}
#endif

// ------------------------------------------ set ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_set_impl(float x, float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;

        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        for (size_t i = 0; i < TOTAL; i += Step)
        {
            op::storeu(out + i, op::set(x));
        }
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_set_impl);

static void kernel_set(float x, float* out) noexcept
{
    TSIMD_DYN_CALL(kernel_set_impl)(x, out);
}

TEST(dyn_dispatch_x86_float32, set)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;

    alignas(ALIGNMENT) float out[TOTAL];

    for (int i = 0; i < TOTAL; ++i) out[i] = -1.0f;

    kernel_set(3.5f, out);

    for (int i = 0; i < TOTAL; ++i)
        EXPECT_FLOAT_EQ(out[i], 3.5f);
}
#endif

// ------------------------------------------ load + store ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_load_store_impl(const float* TSIMD_RESTRICT in, float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;

        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        for (size_t i = 0; i < TOTAL; i += Step)
        {
            op::store(out + i, op::load(in + i));
        }
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_load_store_impl);

static void kernel_load_store(const float* in, float* out) noexcept
{
    TSIMD_DYN_CALL(kernel_load_store_impl)(in, out);
}

TEST(dyn_dispatch_x86_float32, load_store)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;

    alignas(ALIGNMENT) float in[TOTAL], out[TOTAL];

    for (size_t i = 0; i < TOTAL; ++i) in[i] = float(i + 1);
    for (size_t i = 0; i < TOTAL; ++i) out[i] = -1.0f;

    kernel_load_store(in, out);

    for (size_t i = 0; i < TOTAL; ++i)
        EXPECT_FLOAT_EQ(out[i], in[i]);
}
#endif

// ------------------------------------------ loadu + storeu ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_loadu_storeu_impl(const float* TSIMD_RESTRICT in, float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;

        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        for (size_t i = 0; i < TOTAL; i += Step)
        {
            op::storeu(out + i, op::loadu(in + i));
        }
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_loadu_storeu_impl);

static void kernel_loadu_storeu(const float* in, float* out) noexcept
{
    TSIMD_DYN_CALL(kernel_loadu_storeu_impl)(in, out);
}

TEST(dyn_dispatch_x86_float32, loadu_storeu)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;

    alignas(ALIGNMENT) float in[TOTAL], out[TOTAL];

    for (size_t i = 0; i < TOTAL; ++i) in[i] = float(i * 2 + 1);
    for (size_t i = 0; i < TOTAL; ++i) out[i] = -1.0f;

    kernel_loadu_storeu(in, out);

    for (size_t i = 0; i < TOTAL; ++i)
        EXPECT_FLOAT_EQ(out[i], in[i]);
}
#endif

// ------------------------------------------ add ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_add_impl(const float* TSIMD_RESTRICT a, const float* TSIMD_RESTRICT b, float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;

        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        for (size_t i = 0; i < TOTAL; i += Step)
        {
            op::storeu(out + i, op::add(op::loadu(a + i), op::loadu(b + i)));
        }
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_add_impl);

static void kernel_add(const float* a, const float* b, float* out) noexcept
{
    TSIMD_DYN_CALL(kernel_add_impl)(a, b, out);
}

TEST(dyn_dispatch_x86_float32, add)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;

    alignas(ALIGNMENT) float a[TOTAL], b[TOTAL], out[TOTAL];

    for (size_t i = 0; i < TOTAL; ++i)
    {
        a[i] = float(i);
        b[i] = float(100 + i);
        out[i] = -1.0f;
    }

    kernel_add(a, b, out);

    for (size_t i = 0; i < TOTAL; ++i)
        EXPECT_FLOAT_EQ(out[i], a[i] + b[i]);
}
#endif

// ------------------------------------------ sub ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_sub_impl(const float* TSIMD_RESTRICT a, const float* TSIMD_RESTRICT b, float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;

        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        for (size_t i = 0; i < TOTAL; i += Step)
        {
            op::storeu(out + i, op::sub(op::loadu(a + i), op::loadu(b + i)));
        }
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_sub_impl);

static void kernel_sub(const float* a, const float* b, float* out) noexcept
{
    TSIMD_DYN_CALL(kernel_sub_impl)(a, b, out);
}

TEST(dyn_dispatch_x86_float32, sub)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;

    alignas(ALIGNMENT) float a[TOTAL], b[TOTAL], out[TOTAL];

    for (size_t i = 0; i < TOTAL; ++i)
    {
        a[i] = float(i*2 + 10);
        b[i] = float(i);
        out[i] = -1.0f;
    }

    kernel_sub(a, b, out);

    for (size_t i = 0; i < TOTAL; ++i)
        EXPECT_FLOAT_EQ(out[i], a[i] - b[i]);
}
#endif

// ------------------------------------------ mul ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_mul_impl(const float* TSIMD_RESTRICT a, const float* TSIMD_RESTRICT b, float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;

        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        for (size_t i = 0; i < TOTAL; i += Step)
        {
            op::storeu(out + i, op::mul(op::loadu(a + i), op::loadu(b + i)));
        }
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_mul_impl);

static void kernel_mul(const float* a, const float* b, float* out) noexcept
{
    TSIMD_DYN_CALL(kernel_mul_impl)(a, b, out);
}

TEST(dyn_dispatch_x86_float32, mul)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;

    alignas(ALIGNMENT) float a[TOTAL], b[TOTAL], out[TOTAL];

    for (size_t i = 0; i < TOTAL; ++i)
    {
        a[i] = float(i+1);
        b[i] = float(2*(i+1));
        out[i] = -1.0f;
    }

    kernel_mul(a, b, out);

    for (size_t i = 0; i < TOTAL; ++i)
        EXPECT_FLOAT_EQ(out[i], a[i] * b[i]);
}
#endif

// ------------------------------------------ div ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_div_impl(const float* TSIMD_RESTRICT a, const float* TSIMD_RESTRICT b, float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;

        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        for (size_t i = 0; i < TOTAL; i += Step)
        {
            op::storeu(out + i, op::div(op::loadu(a + i), op::loadu(b + i)));
        }
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_div_impl);

static void kernel_div(const float* a, const float* b, float* out) noexcept
{
    TSIMD_DYN_CALL(kernel_div_impl)(a, b, out);
}

TEST(dyn_dispatch_x86_float32, div)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;
    
    alignas(ALIGNMENT) float a[TOTAL], b[TOTAL], out[TOTAL];

    for (size_t i = 0; i < TOTAL; ++i)
    {
        a[i] = float((i+1)*10);
        b[i] = float(i+1);
        out[i] = -1.0f;
    }

    kernel_div(a, b, out);

    for (size_t i = 0; i < TOTAL; ++i)
        EXPECT_FLOAT_EQ(out[i], a[i] / b[i]);
}
#endif

// ------------------------------------------ sum ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_sum_impl(const float* TSIMD_RESTRICT in, float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;
        
        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        float sum = 0.0f;
        for (size_t i = 0; i < TOTAL; i += Step)
        {
            sum += op::reduce_sum(op::loadu(in + i));
        }
        *out = sum;
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_sum_impl);

static float kernel_sum(const float* in) noexcept
{
    float out;
    TSIMD_DYN_CALL(kernel_sum_impl)(in, &out);
    return out;
}

TEST(dyn_dispatch_x86_float32, sum)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;
    
    alignas(ALIGNMENT) float in[TOTAL];

    for (size_t i = 0; i < TOTAL; ++i)
        in[i] = float(i+1); // 1,2,3,...

    float r = kernel_sum(in);

    float expected = 0.0f;
    for (size_t i = 0; i < TOTAL; ++i)
        expected += in[i];

    EXPECT_FLOAT_EQ(r, expected);
}
#endif

// ------------------------------------------ mul_add ------------------------------------------
namespace tsimd::TSIMD_DYN_INSTRUCTION
{
    TSIMD_DYN_FUNC_ATTR
    void kernel_mul_add_impl(
        const float* TSIMD_RESTRICT a,
        const float* TSIMD_RESTRICT b,
        const float* TSIMD_RESTRICT c,
        float* TSIMD_RESTRICT out) noexcept
    {
        constexpr size_t TOTAL = 16;

        using op = TSIMD_CURRENT_OP(float);
        constexpr size_t Step = op::Lanes;

        float sum = 0.0f;
        for (size_t i = 0; i < TOTAL; i += Step)
        {
            sum += op::reduce_sum(op::mul_add(
                op::loadu(a + i),
                op::loadu(b + i),
                op::loadu(c + i)
            ));
        }
        *out = sum;
    }
}

#if TSIMD_ONCE
TSIMD_DYN_DISPATCH_FUNC(kernel_mul_add_impl);

static float kernel_mul_add(const float* a, const float* b, const float* c) noexcept
{
    float out;
    TSIMD_DYN_CALL(kernel_mul_add_impl)(a, b, c, &out);
    return out;
}

TEST(dyn_dispatch_x86_float32, mul_add)
{
    constexpr size_t TOTAL = 16;
    constexpr size_t ALIGNMENT = 32;

    alignas(ALIGNMENT) float a[TOTAL];
    alignas(ALIGNMENT) float b[TOTAL];
    alignas(ALIGNMENT) float c[TOTAL];

    for (size_t i = 0; i < TOTAL; ++i)
    {
        a[i] = float(i + 1);        // 1,2,3,...
        b[i] = float(i + 2);        // 2,3,4,...
        c[i] = float(i + 3);        // 3,4,5,...
    }

    float r = kernel_mul_add(a, b, c);

    float expected = 0.0f;
    for (size_t i = 0; i < TOTAL; ++i)
        expected += a[i] * b[i] + c[i];

    EXPECT_FLOAT_EQ(r, expected);
}
#endif
