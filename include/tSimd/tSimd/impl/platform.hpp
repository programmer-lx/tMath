#pragma once

#include <cstdint>
#include <limits>

// 建议在windows下使用MSVC (测试除外)
#ifndef TSIMD_IS_TESTING
    #if defined(_WIN64) || defined(_WIN32)
        #ifndef _MSC_VER
            #warning "It is recommended to use the MSVC compiler on the Windows platform."
        #endif
    #endif
#endif

// 架构判断 (x86 or arm)
#if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC)
    #define TSIMD_PLATFORM_X86_64
#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #define TSIMD_PLATFORM_ARM
#endif



// SIMD support
// __AVX512F__ -> AVX512 -> __AVX2__ macro -> AVX2 -> FMA3(非MSVC平台需要主动开启，而且是完全独立的指令开关) -> AVX -> SSE4.1(不一定有SSE4.2) -> SSE3 -> SSE2
//                        -> F16C(非MSVC平台需要主动开启，而且是完全独立的指令开关) -> AVX
// SSE4.2 -> SSE4.1 (但是tMath选择不支持SSE4.2)
// __F16C__ macro -> F16C
// __FMA__  macro -> FMA3
// __AVX__ macro -> AVX
// Win64 -> SSE2
// arm -> NEON

// AVX也需要依赖编译开关: MSVC: /arch:AVX, other: -mavx
// 当然，如果开启了AVX2，肯定就支持AVX
#if !defined(TSIMD_USE_AVX) && defined(__AVX__) && !defined(TSIMD_USE_SCALAR)
    #define TSIMD_USE_AVX
#endif

// 非MSVC平台需要手动开启SSE4.1，当然如果支持AVX，也就支持SSE4.1了
#if !defined(TSIMD_USE_SSE4_1) && ( defined(__SSE4_1__) || defined(TSIMD_USE_AVX) ) && !defined(TSIMD_USE_SCALAR)
    #define TSIMD_USE_SSE4_1
#endif

// 非MSVC平台需要手动开启SSE3，当然如果支持SSE4.1，也就支持SSE3了
#if !defined(TSIMD_USE_SSE3) && ( defined(TSIMD_USE_SSE4_1) || defined(__SSE3__) ) && !defined(TSIMD_USE_SCALAR)
    #define TSIMD_USE_SSE3
#endif

// 非MSVC平台需要手动开启SSE2，当然如果支持SSE3，也就支持SSE2了
// x86 64 默认支持 SSE2
#if !defined(TSIMD_USE_SSE2) && ( defined(TSIMD_USE_SSE3) || defined(__SSE2__) || defined(TSIMD_PLATFORM_X86_64) ) && !defined(TSIMD_USE_SCALAR)
    #define TSIMD_USE_SSE2
#endif

// 没有 SSE2 指令，那就使用标量
#if !defined(TSIMD_USE_SSE2) && !defined(TSIMD_USE_SCALAR)
    #define TSIMD_USE_SCALAR
#endif


// call conv
#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(TSIMD_USE_SCALAR) && !defined(TSIMD_VECTORCALL_ENABLED)
    #define TSIMD_VECTORCALL_ENABLED
#endif

#if defined(TSIMD_VECTORCALL_ENABLED)
    #define TSIMD_CALL_CONV __vectorcall
#else
    #define TSIMD_CALL_CONV
#endif
#define TSIMD_SCALAR_CALL_CONV // 统一接口表示方式


// 浮点检查
#if !defined(TSIMD_USE_SCALAR)
    static_assert(sizeof(float) == 4, "tsimd requires 4-byte float");
    static_assert(std::numeric_limits<float>::is_iec559, "tsimd requires IEEE 754 float");

    static_assert(sizeof(double) == 8, "tsimd requires 8-byte double");
    static_assert(std::numeric_limits<double>::is_iec559, "tsimd requires IEEE 754 double");
#endif


// SIMD headers
#ifndef TSIMD_USE_SCALAR
    // SSE2 headers
    #ifdef TSIMD_USE_SSE2
        #include <xmmintrin.h>
        #include <emmintrin.h>

        // AVX header
        #if defined(TSIMD_USE_AVX)
            #include <immintrin.h>
        #endif
    #endif
#endif

// tsimd headers

#include "defs.hpp"

TSIMD_NAMESPACE_BEGIN

// 内存对齐字节数，如果用户开启了AVX512的编译开关，那么就设置为64，开启了AVX，就设置为32，SSE就是16，标量就是4
// 当用户需要使用容器存储数据的时候，如果容器的内存分配器是最大字节对齐的，那么他就可以安全使用load，否则只能使用loadu函数
#if defined(TSIMD_USE_AVX)
    static constexpr size_t RequiredAlignment = 32;
#elif defined(TSIMD_USE_SSE2)
    static constexpr size_t RequiredAlignment = 16;
#elif defined(TSIMD_USE_SCALAR)
    static constexpr size_t RequiredAlignment = 4;
#else
    #error "error"
#endif

TSIMD_NAMESPACE_END
