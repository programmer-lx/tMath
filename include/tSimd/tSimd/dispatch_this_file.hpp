#pragma once

#include "impl/ops/dispatch.hpp"

#if !defined(TSIMD_DISPATCH_THIS_FILE)
    #error "has not defined TSIMD_DISPATCH_THIS_FILE"
#endif


#undef TSIMD_ONCE
#define TSIMD_ONCE 0

// -------------------------------- x86 --------------------------------
#if defined(TSIMD_X86_ANY)

    // AVX2_FMA3
    #undef TSIMD_DYN_INSTRUCTION
    #define TSIMD_DYN_INSTRUCTION AVX2_FMA3
    // 此时 TSIMD_DYN_FUNC_ATTR 等于 AVX2_FMA3
    #undef TSIMD_DYN_FUNC_ATTR
    #define TSIMD_DYN_FUNC_ATTR TSIMD_AVX2_FMA3_INTRINSIC_ATTR
    #include TSIMD_DISPATCH_THIS_FILE // dispatch


    // AVX2
    #undef TSIMD_DYN_INSTRUCTION
    #define TSIMD_DYN_INSTRUCTION AVX2
    // 此时 TSIMD_DYN_FUNC_ATTR 等于 AVX2
    #undef TSIMD_DYN_FUNC_ATTR
    #define TSIMD_DYN_FUNC_ATTR TSIMD_AVX2_INTRINSIC_ATTR
    #include TSIMD_DISPATCH_THIS_FILE // dispatch


    // AVX
    #undef TSIMD_DYN_INSTRUCTION
    #define TSIMD_DYN_INSTRUCTION AVX
    // 此时 TSIMD_DYN_FUNC_ATTR 等于 AVX
    #undef TSIMD_DYN_FUNC_ATTR
    #define TSIMD_DYN_FUNC_ATTR TSIMD_AVX_INTRINSIC_ATTR
    #include TSIMD_DISPATCH_THIS_FILE // dispatch



    // SSE2
    #undef TSIMD_DYN_INSTRUCTION
    #define TSIMD_DYN_INSTRUCTION SSE2
    // 此时 TSIMD_DYN_FUNC_ATTR 等于 SSE2
    #undef TSIMD_DYN_FUNC_ATTR
    #define TSIMD_DYN_FUNC_ATTR TSIMD_SSE2_INTRINSIC_ATTR
    #include TSIMD_DISPATCH_THIS_FILE // dispatch



    // SSE
    #undef TSIMD_DYN_INSTRUCTION
    #define TSIMD_DYN_INSTRUCTION SSE
    // 此时 TSIMD_DYN_FUNC_ATTR 等于 SSE
    #undef TSIMD_DYN_FUNC_ATTR
    #define TSIMD_DYN_FUNC_ATTR TSIMD_SSE_INTRINSIC_ATTR
    #include TSIMD_DISPATCH_THIS_FILE // dispatch


#endif // x86

#if defined(TSIMD_ARM_ANY)

    // ARM
    #undef TSIMD_DYN_INSTRUCTION
    #define TSIMD_DYN_INSTRUCTION NEON
    // 此时 TSIMD_DYN_FUNC_ATTR 等于 NEON
    #undef TSIMD_DYN_FUNC_ATTR
    #define TSIMD_DYN_FUNC_ATTR TSIMD_NEON_INTRINSIC_ATTR
    #include TSIMD_DISPATCH_THIS_FILE // dispatch

#endif // ARM


// last dispatch

// once
#undef TSIMD_ONCE
#define TSIMD_ONCE 1

// Scalar
#undef TSIMD_DYN_INSTRUCTION
#define TSIMD_DYN_INSTRUCTION Scalar
// 此时 TSIMD_DYN_FUNC_ATTR 等于 Scalar
#undef TSIMD_DYN_FUNC_ATTR
#define TSIMD_DYN_FUNC_ATTR TSIMD_SCALAR_INTRINSIC_ATTR
// #include TSIMD_DISPATCH_THIS_FILE // dispatch (不需要include)



#undef TSIMD_DISPATCH_THIS_FILE
