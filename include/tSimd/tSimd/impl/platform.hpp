#pragma once

#include <cstddef>

// compiler detect
#if defined(_MSC_VER) && !defined(__clang__)
    #define TSIMD_COMPILER_MSVC
#elif defined(__GNUC__) && !defined(__clang__)
    #define TSIMD_COMPILER_GCC
#elif defined(__clang__)
    #define TSIMD_COMPILER_CLANG
#else
    #error "Unknown compiler, please use MSVC, GCC, clang."
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


#define TSIMD_STR_IMPL(x) #x
#define TSIMD_STR(x) TSIMD_STR_IMPL(x)

#define TSIMD_CONCAT_IMPL(a, b) a##b
#define TSIMD_CONCAT(a, b) TSIMD_CONCAT_IMPL(a, b)

#if defined(TSIMD_COMPILER_MSVC)
    #define TSIMD_FUNCTION __FUNCSIG__  // function name + template args
    #define TSIMD_RESTRICT __restrict
    #define TSIMD_NOINLINE __declspec(noinline)
    #define TSIMD_FORCE_INLINE __forceinline
    #define TSIMD_FLATTEN
    #define TSIMD_NORETURN __declspec(noreturn)
    #define TSIMD_LIKELY(expr) (expr)
    #define TSIMD_UNLIKELY(expr) (expr)
    #define TSIMD_PRAGMA(tokens) __pragma(tokens)
    #define TSIMD_DIAGNOSTICS(tokens) TSIMD_PRAGMA(warning(tokens))
    #define TSIMD_DIAGNOSTICS_OFF(msvc, gcc) TSIMD_DIAGNOSTICS(msvc)

    #define TSIMD_FUNC_ATTR_INTRINSIC_TARGETS(...)

    #if (_MSC_VER >= 1700)
        #define TSIMD_CHECK_RETURN _Check_return_
    #else
        #define TSIMD_CHECK_RETURN
    #endif
#elif defined(TSIMD_COMPILER_GCC) || defined(TSIMD_COMPILER_CLANG)
    #define TSIMD_FUNCTION __PRETTY_FUNCTION__  // function name + template args
    #define TSIMD_RESTRICT __restrict__
    #define TSIMD_NOINLINE __attribute__((noinline))
    #define TSIMD_FORCE_INLINE inline __attribute__((always_inline))
    #define TSIMD_FLATTEN __attribute__((flatten))
    #define TSIMD_NORETURN __attribute__((noreturn))
    #define TSIMD_LIKELY(expr) __builtin_expect(!!(expr), 1)
    #define TSIMD_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
    #define TSIMD_PRAGMA(tokens) _Pragma(#tokens)
    #define TSIMD_DIAGNOSTICS(tokens) TSIMD_PRAGMA(GCC diagnostic tokens)
    #define TSIMD_DIAGNOSTICS_OFF(msvc, gcc) TSIMD_DIAGNOSTICS(gcc)
    #define TSIMD_FUNC_ATTR_INTRINSIC_TARGETS(...) __attribute__((target(__VA_ARGS__)))
    #define TSIMD_CHECK_RETURN __attribute__((warn_unused_result))
#endif // MSVC





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
