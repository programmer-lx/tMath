#pragma once

#include <cstddef>

// compiler detect
#if defined(_MSC_VER) && !defined(__clang__)
    #define TMATH_COMPILER_MSVC
#elif defined(__GNUC__) && !defined(__clang__)
    #define TMATH_COMPILER_GCC
#elif defined(__clang__)
    #define TMATH_COMPILER_CLANG
#endif

#define TMATH_STR_IMPL(x) #x
#define TMATH_STR(x) TMATH_STR_IMPL(x)

#define TMATH_CONCAT_IMPL(a, b) a##b
#define TMATH_CONCAT(a, b) TMATH_CONCAT_IMPL(a, b)

#define TMATH_NO_DISCARD [[nodiscard]]

#if defined(TMATH_COMPILER_MSVC)

    #define TMATH_FUNCTION __FUNCSIG__  // function name + template args
    #define TMATH_RESTRICT __restrict
    #define TMATH_NOINLINE __declspec(noinline)
    #define TMATH_FORCE_INLINE __forceinline
    #define TMATH_FLATTEN
    #define TMATH_NORETURN __declspec(noreturn)
    #define TMATH_LIKELY(expr) (expr)
    #define TMATH_UNLIKELY(expr) (expr)
    #define TMATH_PRAGMA(tokens) __pragma(tokens)
    #define TMATH_DIAGNOSTICS(tokens) TMATH_PRAGMA(warning(tokens))
    #define TMATH_DIAGNOSTICS_DISABLE(msvc, gcc, clang) TMATH_DIAGNOSTICS(disable : msvc)
    #define TMATH_FUNC_ATTR_INTRINSIC_TARGETS(...)

#elif defined(TMATH_COMPILER_GCC) || defined(TMATH_COMPILER_CLANG) // GCC clang

    #define TMATH_FUNCTION __PRETTY_FUNCTION__  // function name + template args
    #define TMATH_RESTRICT __restrict__
    #define TMATH_NOINLINE __attribute__((noinline))
    #define TMATH_FORCE_INLINE inline __attribute__((always_inline))
    #define TMATH_FLATTEN __attribute__((flatten))
    #define TMATH_NORETURN __attribute__((noreturn))
    #define TMATH_LIKELY(expr) __builtin_expect(!!(expr), 1)
    #define TMATH_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
    #define TMATH_PRAGMA(tokens) _Pragma(#tokens)
    #define TMATH_FUNC_ATTR_INTRINSIC_TARGETS(...) __attribute__((target(__VA_ARGS__)))

    #if !defined(TMATH_COMPILER_CLANG) // GCC only
        #define TMATH_DIAGNOSTICS(tokens) TMATH_PRAGMA(GCC diagnostic tokens)
        #define TMATH_DIAGNOSTICS_DISABLE(msvc, gcc, clang) TMATH_DIAGNOSTICS(ignored gcc)
    #endif

    #if !defined(TMATH_COMPILER_GCC) // clang only
        #define TMATH_DIAGNOSTICS(tokens) TMATH_PRAGMA(clang diagnostic tokens)
        #define TMATH_DIAGNOSTICS_DISABLE(msvc, gcc, clang) TMATH_DIAGNOSTICS(ignored clang)
    #endif

#endif // MSVC