#pragma once

// TODO
#ifndef _WIN64
#error "TODO: The library only support win64 system"
#endif

// C++ check
#ifndef __cplusplus
    #error tMath requires C++
#endif


// SIMD support
// __AVX2__ macro -> AVX2 -> FMA3 -> AVX -> SSE4.1(不一定有SSE4.2) -> SSE3 -> SSE2
//                        -> F16C -> AVX
// SSE4.2 -> SSE4.1 (但是tMath选择不支持SSE4.2)
// __F16C__ macro -> F16C
// __AVX__ macro -> AVX
// Win64 -> SSE2
// arm -> NEON
#if !defined(TMATH_HAS_AVX2) && defined(__AVX2__) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_AVX2
#endif

#if !defined(TMATH_HAS_FMA3) && ( defined(TMATH_HAS_AVX2) || defined(__FMA__) ) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_FMA3
#endif

#if !defined(TMATH_HAS_F16C) && defined(TMATH_HAS_AVX2) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_F16C
#endif

#if !defined(TMATH_HAS_F16C) && defined(__F16C__) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_F16C
#endif

#if defined(TMATH_HAS_FMA3) && !defined(TMATH_HAS_AVX) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_AVX
#endif

#if defined(TMATH_HAS_F16C) && !defined(TMATH_HAS_AVX) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_AVX
#endif

#if !defined(TMATH_HAS_AVX) && defined(__AVX__) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_AVX
#endif

#if defined(TMATH_HAS_AVX) && ( !defined(TMATH_HAS_SSE4_1) || defined(__SSE4_1__) ) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_SSE4_1
#endif

#if defined(TMATH_HAS_SSE4_1) && !defined(TMATH_HAS_SSE3) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_SSE3
#endif

#if defined(TMATH_HAS_SSE3) && !defined(TMATH_HAS_SSE2) && !defined(TMATH_NO_SIMD)
    #define TMATH_HAS_SSE2
#endif

// X86_64平台默认启用SSE2, ARM平台默认启用NEON
#if !defined(TMATH_HAS_ARM_NEON) && !defined(TMATH_HAS_SSE2) && !defined(TMATH_NO_SIMD)
    #if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC)
        #define TMATH_HAS_SSE2
    #elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __arm__ || __aarch64__
        #define TMATH_HAS_ARM_NEON
    #elif !defined(TMATH_NO_SIMD)
        #error tMath does not support this target
    #endif
#endif // !TMATH_HAS_ARM_NEON && !TMATH_HAS_SSE2 && !TMATH_NO_SIMD

// Intel SVML 数学运算指令集
#if defined(TMATH_HAS_SSE2) && defined(_MSC_VER) && (_MSC_VER >= 1920) && !defined(__clang__) && !defined(TMATH_HAS_SVML) && !defined(TMATH_DISABLE_SVML)
    #define TMATH_HAS_SVML
#endif




// for testing
#ifdef TMATH_IS_TESTING

#undef TMATH_HAS_SSE2
#undef TMATH_HAS_SSE3
#undef TMATH_HAS_SSE4_1
#undef TMATH_HAS_AVX
#undef TMATH_HAS_AVX2
#undef TMATH_HAS_FMA3
#undef TMATH_HAS_F16C

#if defined(TMATH_TEST_SSE2)
    #define TMATH_HAS_SSE2
#endif
#if defined(TMATH_TEST_SSE3)
    #define TMATH_HAS_SSE3
#endif
#if defined(TMATH_TEST_SSE4_1)
    #define TMATH_HAS_SSE4_1
#endif
#if defined(TMATH_TEST_AVX)
    #define TMATH_HAS_AVX
#endif
#if defined(TMATH_TEST_AVX2)
    #define TMATH_HAS_AVX2
#endif
#if defined(TMATH_TEST_FMA3)
    #define TMATH_HAS_FMA3
#endif
#if defined(TMATH_TEST_F16C)
    #define TMATH_HAS_F16C
#endif

#endif



// SIMD headers
#ifndef TMATH_NO_SIMD

    // SSE2 headers
    #ifdef TMATH_HAS_SSE2
        #include <xmmintrin.h>
        #include <emmintrin.h>

        // SSE3 header
        #ifdef TMATH_HAS_SSE3
            #include <pmmintrin.h>
        #endif

        // SSE4.1 header
        #ifdef TMATH_HAS_SSE4_1
            #include <smmintrin.h>
        #endif

        // AVX header
        #ifdef TMATH_HAS_AVX
            #include <immintrin.h>
        #endif

    // ARM NEON header
    #elif defined(TMATH_HAS_ARM_NEON)
        #if defined(_MSC_VER) && !defined(__clang__) && (defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC))
            #include <arm64_neon.h>
        #else
            #include <arm_neon.h>
        #endif
    #endif
#endif
