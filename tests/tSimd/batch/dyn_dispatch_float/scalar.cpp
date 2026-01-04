#define TSIMD_TEST_SCALAR

#include "dyn_dispatch_float.inl"


TSIMD_DYN_DISPATCH_FUNC(, void, , dispatch_check, (float* data, size_t n, float& out_sum), )
{
    using op = tsimd::SimdOp<CurrentSimdInstruction, float>;
    using batch_t = op::batch_t;
    constexpr auto Step = op::Lanes;

    EXPECT_TRUE(op::BatchAlignment == 4);
    EXPECT_TRUE(Step == 1);
    EXPECT_TRUE(op::BatchSize == 4);
    EXPECT_TRUE(op::ElementSize == 4);
    EXPECT_TRUE(CurrentSimdInstruction == tsimd::SimdInstruction::Scalar);

    batch_t sum_n = op::zero();

    size_t i = 0;
    for (; i + Step <= n; i += Step)
    {
        batch_t tmp = op::loadu(data + i);
        sum_n = op::add(sum_n, tmp);
    }

    // 处理剩余数字
    float sum = op::sum(sum_n);
    for (; i < n; ++i)
    {
        sum += data[i];
    }

    out_sum = sum;
}

TEST(scalar, dispatch_check)
{
    init_dyn_selector_if_not();

    float numbers[] = { 1, 2, 3, 4, 5 };

    float sum = 0.0f;
    dispatch_check()(numbers, 5, sum);

    EXPECT_NEAR(sum, 15.0f, 1e-5f);
}

int main(int argc, char **argv)
{
    tsimd::InstructionSelector::init();
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}