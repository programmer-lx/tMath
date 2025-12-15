#pragma once

#include "math_defs_simd.hpp"

// only support x86_64 os
static_assert(sizeof(void*) == 8, "only support x86_64 system");

#if !defined(__x86_64__) && !defined(_M_X64)
#error "only support x86_64 system"
#endif



// SIMD support
#define TMATH_SIMD_USE_SSE2 0
#define TMATH_SIMD_USE_AVX2 0
#ifdef _MSC_VER

    #ifdef _M_X64
        #undef TMATH_SIMD_USE_SSE2
        #define TMATH_SIMD_USE_SSE2 1
    #endif

    #ifdef __AVX2__
        #undef TMATH_SIMD_USE_AVX2
        #define TMATH_SIMD_USE_AVX2 1
    #endif

#elif defined (__GNUC__) || defined (__clang__)

    #ifdef __SSE2__
        #undef TMATH_SIMD_USE_SSE2
        #define TMATH_SIMD_USE_SSE2 1
    #endif

    #ifdef __AVX2__
        #undef TMATH_SIMD_USE_AVX2
        #define TMATH_SIMD_USE_AVX2 1
    #endif

#endif


// 所有SIMD指令均不可用，编译失败
#if ((!TMATH_SIMD_USE_SSE2) && (!TMATH_SIMD_USE_AVX2))
    #error "SIMD is disabled, you should use the `tMath` library instead of `tSimd`"
#endif



// runtime check SIMD support
#ifdef _MSC_VER
    #include <intrin.h>
    #define TMATH_CPUID_CALL(cpui, leaf, subleaf) __cpuidex((cpui), (leaf), (subleaf))
#elif defined(__GNUC__) || defined(__clang__)
    #include <cpuid.h>

    inline void __cpuidex_custom(int cpui[4], int leaf, int subleaf) {
        __cpuid_count(leaf, subleaf,
                      reinterpret_cast<unsigned int&>(cpui[0]),
                      reinterpret_cast<unsigned int&>(cpui[1]),
                      reinterpret_cast<unsigned int&>(cpui[2]),
                      reinterpret_cast<unsigned int&>(cpui[3]));
    }
    #define TMATH_CPUID_CALL(cpui, leaf, subleaf) __cpuidex_custom(cpui, leaf, subleaf)

    inline unsigned long long _xgetbv(unsigned int index) {
        unsigned int eax, edx;
        __asm__ __volatile__(".byte 0x0f, 0x01, 0xd0" : "=a"(eax), "=d"(edx) : "c"(index));
        return (static_cast<unsigned long long>(edx) << 32) | eax;
    }
#else
    #error "tSimd: Unsupported compiler for SIMD CPUID checks."
#endif



TMATH_SIMD_NAMESPACE_BEGIN

inline bool supports_SSE2() noexcept
{
#if TMATH_SIMD_USE_SSE2

    int cpui[4] = {0};
    TMATH_CPUID_CALL(cpui, 1, 0);
    return (cpui[3] & 0x04000000) != 0;

#else
    return false;
#endif
}

inline bool supports_AVX2() noexcept
{
#if TMATH_SIMD_USE_AVX2

    int cpui[4] = {0};
    int nIds = 0;

    // A. 检查最高功能叶 ID
    TMATH_CPUID_CALL(cpui, 0, 0);
    nIds = cpui[0];
    if (nIds < 7)
    {
        return false; // 不支持 Leaf 7 (AVX2 信息)
    }

    // B. 检查 Leaf 1: 必须有 OSXSAVE (Bit 27) 和 AVX (Bit 28)
    TMATH_CPUID_CALL(cpui, 1, 0);
    const bool has_osxsave = (cpui[2] & 0x08000000) != 0; // OSXSAVE (ECX Bit 27)
    const bool has_avx = (cpui[2] & 0x10000000) != 0;     // AVX (ECX Bit 28)

    if (!has_osxsave || !has_avx)
    {
        return false;
    }

    // C. 检查 XCR0 寄存器 (操作系统是否保存 AVX 状态)
    // XCR0 的 Bit 1 (SSE) 和 Bit 2 (AVX) 必须设置
    if ((_xgetbv(0) & 0x6) != 0x6) // 0x6 = 0b110
    {
        return false;
    }

    // D. 检查 Leaf 7: AVX2 位
    TMATH_CPUID_CALL(cpui, 7, 0);

    // AVX2 位在 EBX 的 Bit 5 (0x00000020)
    return (cpui[1] & 0x00000020) != 0;

#else
        return false;
#endif
}

TMATH_SIMD_NAMESPACE_END
