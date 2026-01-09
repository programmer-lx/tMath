#pragma once

// clang-format off

#include "impl/platform.hpp"

// 判断不同的平台，引入不同的backend

// Scalar
#if defined(TSIMD_INSTRUCTION_FEATURE_SCALAR)
    #include "impl/ops/Scalar/Scalar_float.hpp"
#endif


// SSE
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE)
    #include "impl/ops/x86/SSE_family/SSE_float32.hpp"
#endif

// SSE2
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE2)
    #include "impl/ops/x86/SSE_family/SSE2_float32.hpp"
#endif

// SSE3
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE3)
    #include "impl/ops/x86/SSE_family/SSE3_float32.hpp"
#endif

// SSE4.1
#if defined(TSIMD_INSTRUCTION_FEATURE_SSE4_1)
    #include "impl/ops/x86/SSE_family/SSE4_1_float32.hpp"
#endif


// AVX
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX)
    #include "impl/ops/x86/AVX_family/AVX_float32.hpp"
#endif

// AVX2
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX2)
    #include "impl/ops/x86/AVX_family/AVX2_float32.hpp"
#endif

// AVX2 + FMA3
#if defined(TSIMD_INSTRUCTION_FEATURE_AVX2) && defined(TSIMD_INSTRUCTION_FEATURE_FMA3)
    #include "impl/ops/x86/AVX_family/AVX2_FMA3_float32.hpp"
#endif

// clang-format on
