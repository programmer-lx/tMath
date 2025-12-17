#pragma once


// #ifndef _WIN64
// #error "The library only support win64 system"
// #endif

// C++ check
#ifndef __cplusplus
    #error tMath requires C++
#endif


// for testing
#ifdef TMATH_IS_TESTING

#undef __SSE2__
#undef __SSE3__
#undef __SSE4_1__
#undef __AVX__
#undef __AVX2__
#undef __FMA__
#undef __F16C__

#if defined(TMATH_TEST_SSE2)
    #define TMATH_USE_SSE2
#endif

#if defined(TMATH_TEST_SSE3)
    #define TMATH_USE_SSE3
#endif

#if defined(TMATH_TEST_SSE4_1)
    #define TMATH_USE_SSE4_1
#endif

#if defined(TMATH_TEST_AVX)
    #define TMATH_USE_AVX
#endif

#if defined(TMATH_TEST_AVX2)
    #define TMATH_USE_AVX2
#endif

#if defined(TMATH_TEST_FMA3)
    #define TMATH_USE_FMA3
#endif

#if defined(TMATH_TEST_F16C)
    #define TMATH_USE_F16C
#endif

#endif


// SIMD support
// __AVX2__ macro -> AVX2 -> FMA3 -> AVX -> SSE4.1(不一定有SSE4.2) -> SSE3 -> SSE2
//                        -> F16C -> AVX
// SSE4.2 -> SSE4.1 (但是tMath选择不支持SSE4.2)
// __F16C__ macro -> F16C
// __AVX__ macro -> AVX
// Win64 -> SSE2
// arm -> NEON
#if !defined(TMATH_USE_AVX2) && defined(__AVX2__) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_AVX2
#endif

#if !defined(TMATH_USE_FMA3) && ( defined(TMATH_USE_AVX2) || defined(__FMA__) ) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_FMA3
#endif

#if !defined(TMATH_USE_F16C) && defined(TMATH_USE_AVX2) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_F16C
#endif

#if !defined(TMATH_USE_F16C) && defined(__F16C__) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_F16C
#endif

#if defined(TMATH_USE_FMA3) && !defined(TMATH_USE_AVX) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_AVX
#endif

#if defined(TMATH_USE_F16C) && !defined(TMATH_USE_AVX) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_AVX
#endif

#if !defined(TMATH_USE_AVX) && defined(__AVX__) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_AVX
#endif

#if defined(TMATH_USE_AVX) && ( !defined(TMATH_USE_SSE4_1) || defined(__SSE4_1__) ) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_SSE4_1
#endif

#if defined(TMATH_USE_SSE4_1) && ( !defined(TMATH_USE_SSE3) || defined(__SSE3__) ) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_SSE3
#endif

#if defined(TMATH_USE_SSE3) && !defined(TMATH_USE_SSE2) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_SSE2
#endif

// X86_64平台默认启用SSE2, ARM平台默认启用NEON
#if !defined(TMATH_USE_ARM_NEON) && !defined(TMATH_USE_SSE2) && !defined(TMATH_NO_SIMD)
    #if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC)
        #define TMATH_USE_SSE2
    #elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __arm__ || __aarch64__
        #define TMATH_USE_ARM_NEON
    #elif !defined(TMATH_NO_SIMD)
        #error tMath does not support this target
    #endif
#endif // !TMATH_USE_ARM_NEON && !TMATH_USE_SSE2 && !TMATH_NO_SIMD

// Intel SVML 数学运算指令集
#if defined(TMATH_USE_SSE2) && defined(_MSC_VER) && (_MSC_VER >= 1920) && !defined(__clang__) && !defined(TMATH_USE_SVML) && !defined(TMATH_DISABLE_SVML)
    #define TMATH_USE_SVML
#endif



// SIMD headers
#ifndef TMATH_NO_SIMD

    // SSE2 headers
    #ifdef TMATH_USE_SSE2
        #include <xmmintrin.h>
        #include <emmintrin.h>

        // SSE3 header
        #ifdef TMATH_USE_SSE3
            #include <pmmintrin.h>
        #endif

        // SSE4.1 header
        #ifdef TMATH_USE_SSE4_1
            #include <smmintrin.h>
        #endif

        // AVX header
        #ifdef TMATH_USE_AVX
            #include <immintrin.h>
        #endif

    // ARM NEON header
    #elif defined(TMATH_USE_ARM_NEON)
        #if defined(_MSC_VER) && !defined(__clang__) && (defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC))
            #include <arm64_neon.h>
        #else
            #include <arm_neon.h>
        #endif
    #endif
#endif



// SIMD constants
TMATH_SIMD_NAMESPACE_BEGIN
#define TMATH_DEFINE_SIMD_CONSTEXPR(name, value) static constexpr bool name = value

#if defined(TMATH_USE_SSE2)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE2, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE2, false);
#endif

#if defined(TMATH_USE_SSE3)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE3, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE3, false);
#endif

#if defined(TMATH_USE_SSE4_1)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE4_1, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE4_1, false);
#endif

#if defined(TMATH_USE_AVX)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_AVX, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_AVX, false);
#endif

#if defined(TMATH_USE_AVX2)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_AVX2, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_AVX2, false);
#endif

#if defined(TMATH_USE_FMA3)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_FMA3, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_FMA3, false);
#endif

#if defined(TMATH_USE_F16C)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_F16C, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_F16C, false);
#endif

TMATH_SIMD_NAMESPACE_END


// checker for testing
#if defined (TMATH_IS_TESTING)

#include <initializer_list>


// 如果这个宏被定义，就代表库的代码已经被包含
// 对于测试，我们严格规定，测试代码必须在库代码之后，所以测试代码开头一定要检查这个宏有没有定义，如果没有定义，则给出编译报错
#define TMATH_CHECK_TSIMD_INCLUDE


TMATH_SIMD_NAMESPACE_BEGIN

// 用于测试宏是否开关正确
// 测试opened数组是否全部为true，closed数组是否全部为false
constexpr bool test_simd_intrinsics(std::initializer_list<bool> opened, std::initializer_list<bool> closed)
{
    for (bool b : opened)
    {
        if (!b)
        {
            return false;
        }
    }
    for (bool b : closed)
    {
        if (b)
        {
            return false;
        }
    }
    return true;
}

// usage for testing

// no SIMD
// static_assert(test_simd_intrinsics({}, {use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));

// test SSE2
// static_assert(test_simd_intrinsics({use_SSE2}, {use_SSE3, use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));

// teste SSE3
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3}, {use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));

// teste SSE4_1
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1}, {use_AVX, use_AVX2, use_FMA3, use_F16C}));

// teste AVX
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX}, {use_AVX2, use_FMA3, use_F16C}));

// teste FMA3
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_FMA3}, {use_AVX2, use_F16C}));

// teste F16C
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_F16C}, {use_AVX2, use_FMA3}));

// teste AVX2
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_F16C, use_FMA3, use_AVX2}, {}));

TMATH_SIMD_NAMESPACE_END

#endif
