#pragma once

#include <cstddef>

// --- X86 系列 ---
#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
    #define TSIMD_X86_64
    #define TSIMD_X86_ANY
#endif



// SIMD support
// __AVX512F__ -> AVX512 -> __AVX2__ macro -> AVX2 -> FMA3(非MSVC平台需要主动开启，而且是完全独立的指令开关) -> AVX -> SSE4.1(不一定有SSE4.2) -> SSE3 -> SSE2
//                        -> F16C(非MSVC平台需要主动开启，而且是完全独立的指令开关) -> AVX
// SSE4.2 -> SSE4.1 (但是tMath选择不支持SSE4.2)
// __F16C__ macro -> F16C
// __FMA__  macro -> FMA3
// __AVX__ macro -> AVX
// x86 64 -> SSE2
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
