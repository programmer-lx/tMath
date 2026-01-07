#include <tSimd/impl/platform.hpp>

#ifdef TSIMD_ARM_ANY

#undef TSIMD_DISPATCH_THIS_FILE
#define TSIMD_DISPATCH_THIS_FILE "batch/dyn_dispatch_float/ARM/NEON_float32.cpp" // this file
#include <tSimd/dispatch_this_file.hpp> // auto dispatch (在tSimd/batch.hpp前面)
#include <tSimd/batch.hpp> // 一定要在 tSimd/dispatch_this_file.hpp 后面

#include <arm_neon.h>
#include "../test.hpp"

#if TSIMD_ONCE
int main()
{
    return 0;
}
#endif

#else
// x86
int main()
{
    return 0;
}
#endif