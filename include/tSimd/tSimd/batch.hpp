#pragma once

// clang-format off

#include "impl/platform.hpp"

// 判断不同的平台，引入不同的backend

// x86
#if defined(TSIMD_X86_ANY)
    // SSE family
    #include "impl/ops/x86/SSE_family/SSE2_float32.hpp"

    // AVX family
    #include "impl/ops/x86/AVX_family/AVX_float32.hpp"

    // AVX-512 family
    #include "impl/ops/x86/AVX512_family/AVX512_float32.hpp"
#endif

// clang-format on
