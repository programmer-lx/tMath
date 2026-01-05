#pragma once

// clang-format off

#include "impl/platform.hpp"

// 判断不同的平台，引入不同的backend
#include "impl/ops/scalar_float.hpp"

// x86 指令集
#if defined(TSIMD_X86_ANY)
    #include "impl/ops/SSE2_float.hpp"
    #include "impl/ops/AVX_float.hpp"
#endif

// clang-format on
