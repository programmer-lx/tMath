#include <tSimd/batch.hpp>

#include "../../test.hpp"

TEST(cpuid, support)
{

    auto result = tsimd::InstructionSelector::get_support_info();

    EXPECT_TRUE(result.SSE2 == true);
    EXPECT_TRUE(result.AVX == true);
}

int main(int argc, char **argv)
{
    tsimd::InstructionSelector::init();
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}