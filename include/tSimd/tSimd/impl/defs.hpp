#pragma once

#define TSIMD_NAMESPACE_NAME tsimd
#define TSIMD_NAMESPACE_BEGIN namespace TSIMD_NAMESPACE_NAME {
#define TSIMD_NAMESPACE_END }


#define TSIMD_STR_IMPL(x) #x
#define TSIMD_STR(x) TSIMD_STR_IMPL(x)

#define TSIMD_CONCAT_IMPL(a, b) a##b
#define TSIMD_CONCAT(a, b) TSIMD_CONCAT_IMPL(a, b)

#if defined(_MSC_VER) && !defined(__clang__)
    #define TSIMD_COMPILER_MSVC
#elif defined(__GNUC__) || defined(__clang__)
    #define TSIMD_COMPILER_GCC_CLANG
#endif

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
#elif defined(TSIMD_COMPILER_GCC_CLANG) // other
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
#else
    #error "Unknown compiler."
#endif // MSVC