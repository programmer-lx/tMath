#pragma once

#include "../common_macros.hpp"

// compiler
#if !defined(TMATH_COMPILER_MSVC) && !defined(TMATH_COMPILER_GCC) && !defined(TMATH_COMPILER_CLANG)
    #error "Unknown compiler, tSimd only support MSVC, GCC, clang"
#endif


// --- X86 系列 ---
// ----------------------------- x86 64-bit -----------------------------
#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
    #define TSIMD_X86_64
    #define TSIMD_X86_ANY
// ----------------------------- x86 32-bit -----------------------------
#elif defined(_M_IX86) || defined(__i386__)
    #define TSIMD_X86_32
    #define TSIMD_X86_ANY
#else
    #error "Unknown arch, tSimd can only support x86 arch."
#endif



// SIMD support
// AVX512 -> AVX2 -> FMA3(独立的指令开关) -> AVX -> SSE4.1(不一定有SSE4.2) -> SSE3 -> SSE2
//                -> F16C(独立的指令开关) -> AVX
// SSE4.2 -> SSE4.1
// x86 64 -> SSE2


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



#define TSIMD_NAMESPACE_NAME tsimd
#define TSIMD_NAMESPACE_BEGIN namespace TSIMD_NAMESPACE_NAME {
#define TSIMD_NAMESPACE_END }


#define TSIMD_ALIGNMENT_SCALAR 4
#define TSIMD_ALIGNMENT_SSE_FAMILY 16
#define TSIMD_ALIGNMENT_AVX_FAMILY 32
#define TSIMD_ALIGNMENT_AVX512_FAMILY 64

TSIMD_NAMESPACE_BEGIN

namespace Alignment
{
    static constexpr size_t Scalar = TSIMD_ALIGNMENT_SCALAR;
    static constexpr size_t SSE_Family = TSIMD_ALIGNMENT_SSE_FAMILY;
    static constexpr size_t AVX_Family = TSIMD_ALIGNMENT_AVX_FAMILY;
    static constexpr size_t AVX512_Family = TSIMD_ALIGNMENT_AVX512_FAMILY;
}

TSIMD_NAMESPACE_END
