#pragma once

// clang-format off

#include "impl/platform.hpp"

// 判断不同的平台，引入不同的backend
#include "impl/ops/scalar_float.hpp"

#if defined(TSIMD_USE_SSE2)
    #include "impl/ops/SSE2_float.hpp"
#endif

#if defined(TSIMD_USE_AVX)
    #include "impl/ops/AVX_float.hpp"
#endif

// clang-format on
