#pragma once

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


// call conv
#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(TSIMD_VECTORCALL_ENABLED)
    #define TSIMD_VECTORCALL_ENABLED
#endif

#if defined(TSIMD_VECTORCALL_ENABLED)
    #define TSIMD_CALL_CONV __vectorcall
#else
    #define TSIMD_CALL_CONV
#endif
#define TSIMD_SCALAR_CALL_CONV // 统一接口表示方式



// SIMD headers
#if defined(TSIMD_PLATFORM_X86_64)
    #include <xmmintrin.h>
    #include <emmintrin.h>
    #include <immintrin.h>
#endif

// tsimd headers

#include "defs.hpp"
