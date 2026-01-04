#pragma once

#define TSIMD_NAMESPACE_NAME tsimd
#define TSIMD_NAMESPACE_BEGIN namespace TSIMD_NAMESPACE_NAME {
#define TSIMD_NAMESPACE_END }

#if defined(_MSC_VER)
    // MSVC 强制内联
    #define TSIMD_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    // GCC, Clang, 以及基于这些内核的编译器
    #define TSIMD_FORCE_INLINE inline __attribute__((always_inline))
#else
    // 降级到标准 inline
    #define TSIMD_FORCE_INLINE inline
#endif
