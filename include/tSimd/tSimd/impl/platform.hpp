#pragma once

// 建议在windows下使用MSVC (测试除外)
#ifndef TSIMD_IS_TESTING
    #if defined(_WIN64) || defined(_WIN32)
        #if !defined(_MSC_VER) || defined(__clang__)
            #warning "It is recommended to use the MSVC compiler on the Windows platform."
        #endif
    #endif
#endif

// --- X86 系列 ---
#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
    #define TSIMD_X86_64
    #define TSIMD_X86_ANY
#elif defined(_M_IX86) || defined(__i386__) || defined(__i386)
    #define TSIMD_X86_32
    #define TSIMD_X86_ANY
#endif

// --- ARM 系列 ---
#if defined(__aarch64__) || defined(_M_ARM64)
    #define TSIMD_ARM_64
    #define TSIMD_ARM_ANY
#elif defined(__arm__) || defined(_M_ARM)
    #define TSIMD_ARM_32
    #define TSIMD_ARM_ANY
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


// tsimd headers

#include "defs.hpp"
