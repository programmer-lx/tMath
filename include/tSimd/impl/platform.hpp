#pragma once

#include <cstdint>

#include "defs.hpp"

// 建议在windows下使用MSVC (测试除外)
#ifndef TSIMD_IS_TESTING
    #if defined(_WIN64) || defined(_WIN32)
        #ifndef _MSC_VER
            #warning "It is recommended to use the MSVC compiler on the Windows platform."
        #endif
    #endif
#endif

// 架构判断 (x86 or arm)
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #define TSIMD_PLATFORM_X86
#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #define TSIMD_PLATFORM_ARM
#endif


// call conv
#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(TSIMD_NO_SIMD) && !defined(TSIMD_VECTORCALL_ENABLED)
    #define TSIMD_VECTORCALL_ENABLED
#endif

#if defined(TSIMD_VECTORCALL_ENABLED)
    #define TSIMD_SIMD_CALL_CONV __vectorcall
#else
    #define TSIMD_SIMD_CALL_CONV
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
#if !defined(TSIMD_USE_AVX) && defined(__AVX__) && !defined(TSIMD_NO_SIMD)
    #define TSIMD_USE_AVX
#endif

// 非MSVC平台需要手动开启SSE4.1，当然如果支持AVX，也就支持SSE4.1了
#if !defined(TSIMD_USE_SSE4_1) && ( defined(__SSE4_1__) || defined(TSIMD_USE_AVX) ) && !defined(TSIMD_NO_SIMD)
    #define TSIMD_USE_SSE4_1
#endif

// 非MSVC平台需要手动开启SSE3，当然如果支持SSE4.1，也就支持SSE3了
#if !defined(TSIMD_USE_SSE3) && ( defined(TSIMD_USE_SSE4_1) || defined(__SSE3__) ) && !defined(TSIMD_NO_SIMD)
    #define TSIMD_USE_SSE3
#endif

// 非MSVC平台需要手动开启SSE2，当然如果支持SSE3，也就支持SSE2了
#if !defined(TSIMD_USE_SSE2) && ( defined(TSIMD_USE_SSE3) || defined(__SSE2__) ) && !defined(TSIMD_NO_SIMD)
    #define TSIMD_USE_SSE2
#endif

// X86_64平台默认启用SSE2, ARM平台默认启用NEON
#if !defined(TSIMD_USE_ARM_NEON) && !defined(TSIMD_USE_SSE2) && !defined(TSIMD_NO_SIMD)
    #if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC)
        #define TSIMD_USE_SSE2
    #elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __arm__ || __aarch64__
        #define TSIMD_USE_ARM_NEON
    #elif !defined(TSIMD_NO_SIMD)
        #define TSIMD_NO_SIMD /* no simd */
    #endif
#endif // !TSIMD_USE_ARM_NEON && !TSIMD_USE_SSE2 && !TSIMD_NO_SIMD


// 浮点检查
#if !defined(TSIMD_NO_SIMD)
    static_assert(sizeof(float) == 4, "tsimd requires 4-byte float");
    static_assert(std::numeric_limits<float>::is_iec559, "tsimd requires IEEE 754 float");

    static_assert(sizeof(double) == 8, "tsimd requires 8-byte double");
    static_assert(std::numeric_limits<double>::is_iec559, "tsimd requires IEEE 754 double");
#endif


// SIMD headers
#ifndef TSIMD_NO_SIMD
    // SSE2 headers
    #ifdef TSIMD_USE_SSE2
        #include <xmmintrin.h>
        #include <emmintrin.h>

        // AVX header
        #if defined(TSIMD_USE_AVX) || defined(TSIMD_USE_FMA3) || defined(TSIMD_USE_F16C)
            #include <immintrin.h>
        #endif
    #endif
#endif


// checker
TSIMD_NAMESPACE_BEGIN

#if defined(_MSC_VER)
    #include <intrin.h>
namespace detail
{
    inline void cpuidex(int regs[4], int eax, int ecx)
    {
        __cpuidex(regs, eax, ecx);
    }

    inline uint64_t xgetbv_0()
    {
        return _xgetbv(0);
    }
}

#elif defined(__GNUC__) || defined(__clang__)
    #include <cpuid.h>
namespace detail
{
    inline void cpuidex(int regs[4], int eax, int ecx)
    {
        unsigned int a, b, c, d;
        __cpuid_count(eax, ecx, a, b, c, d);
        regs[0] = a;
        regs[1] = b;
        regs[2] = c;
        regs[3] = d;
    }

    inline uint64_t xgetbv_0()
    {
        uint32_t eax, edx;
        __asm__ volatile (
            "xgetbv"
            : "=a"(eax), "=d"(edx)
            : "c"(0)
        );
        return (static_cast<uint64_t>(edx) << 32) | eax;
    }
}
#endif

struct IntrinsicsCheckResult
{
    bool SSE2   = false;
    bool AVX    = false;
};

inline IntrinsicsCheckResult runtime_check_simd_intrinsics() noexcept
{
    IntrinsicsCheckResult result{};

    int regs[4] = { -1 };

    // ===== CPUID.(EAX=1) =====
    detail::cpuidex(regs, 1, 0);

    const bool has_sse2   = (regs[3] & (1 << 26)) != 0;
    const bool has_sse3   = (regs[2] & (1 <<  0)) != 0;
    const bool has_sse41  = (regs[2] & (1 << 19)) != 0;
    const bool has_avx    = (regs[2] & (1 << 28)) != 0;
    const bool has_fma    = (regs[2] & (1 << 12)) != 0;
    const bool has_f16c   = (regs[2] & (1 << 29)) != 0;
    const bool has_osxsave= (regs[2] & (1 << 27)) != 0;

    result.SSE2   = has_sse2;

    // ===== OS 是否支持 AVX 状态保存 =====
    bool os_supports_avx = false;
    if (has_osxsave)
    {
        uint64_t xcr0 = detail::xgetbv_0();
        // XMM (bit 1) + YMM (bit 2)
        os_supports_avx = (xcr0 & 0x6) == 0x6;
    }

    if (has_avx && os_supports_avx)
    {
        result.AVX  = true;
    }

    return result;
}

TSIMD_NAMESPACE_END
