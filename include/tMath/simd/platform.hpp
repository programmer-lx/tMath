#pragma once


// #ifndef _WIN64
// #error "The library only support win64 system"
// #endif

// C++ check
#ifndef __cplusplus
    #error tMath requires C++
#endif

// 建议在windows下使用MSVC (测试除外)
#ifndef TMATH_IS_TESTING
    #if defined(_WIN64) || defined(_WIN32)
        #ifndef _MSC_VER
            #warning "It is recommended to use the MSVC compiler on the Windows platform."
        #endif
    #endif
#endif

// 架构判断 (x86 or arm)
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #define TMATH_PLATFORM_X86
#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #define TMATH_PLATFORM_ARM
#endif


// for testing
// BUT if is doing benchmark, don't enable this
// 测试时，应开启全部的SIMD开关，确保电脑支持所有使用到的SIMD指令
#if defined(TMATH_IS_TESTING) && !defined(TMATH_IS_DOING_BENCHMARK)

    #undef __SSE2__
    #undef __SSE3__
    #undef __SSE4_1__
    #undef __AVX__
    #undef __AVX2__
    #undef __FMA__
    #undef __F16C__

    // 如果是MSVC，则代表开启了AVX2
    #if defined(TMATH_TEST_FMA3)
        #define TMATH_USE_FMA3
        #if defined(_MSC_VER)
            #define TMATH_USE_AVX2
        #endif
    #endif

    // 如果是MSVC，则代表开启了AVX2
    #if defined(TMATH_TEST_F16C)
        #define TMATH_USE_F16C
        #if defined(_MSC_VER)
            #define TMATH_USE_AVX2
        #endif
    #endif

    #if defined(TMATH_TEST_SSE2)
        #define TMATH_USE_SSE2
    #endif

    #if defined(TMATH_TEST_SSE3)
        #define TMATH_USE_SSE3
    #endif

    #if defined(TMATH_TEST_SSE4_1)
        #define TMATH_USE_SSE4_1
    #endif

    #if defined(TMATH_TEST_AVX)
        #define TMATH_USE_AVX
    #endif

    #if defined(TMATH_TEST_AVX2)
        #define TMATH_USE_AVX2
    #endif

#endif // !TMATH_IS_TESTING



// call conv
#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(TMATH_NO_SIMD) && !defined(TMATH_VECTORCALL_ENABLED)
    #define TMATH_VECTORCALL_ENABLED
#endif

#if defined(TMATH_VECTORCALL_ENABLED)
    #define TMATH_SIMD_CALL_CONV __vectorcall
#else
    #define TMATH_SIMD_CALL_CONV
#endif



// SIMD support
// __AVX2__ macro -> AVX2 -> FMA3(非MSVC平台需要主动开启，而且是完全独立的指令开关) -> AVX -> SSE4.1(不一定有SSE4.2) -> SSE3 -> SSE2
//                        -> F16C(非MSVC平台需要主动开启，而且是完全独立的指令开关) -> AVX
// SSE4.2 -> SSE4.1 (但是tMath选择不支持SSE4.2)
// __F16C__ macro -> F16C
// __FMA__  macro -> FMA3
// __AVX__ macro -> AVX
// Win64 -> SSE2
// arm -> NEON

// AVX2需要依赖编译开关: MSVC: /arch:AVX2, other: -mavx2
#if !defined(TMATH_USE_AVX2) && defined(__AVX2__) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_AVX2
#endif

// 非MSVC平台需要独立开启FMA3
#if !defined(_MSC_VER) && !defined(TMATH_USE_FMA3) && defined(__FMA__) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_FMA3
#endif

// MSVC 平台没有FMA3宏，需要开启AVX2
#if defined(_MSC_VER) && !defined(TMATH_USE_FMA3) && defined(TMATH_USE_AVX2) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_FMA3
#endif

// 非MSVC平台需要独立开启F16C
#if !defined(_MSC_VER) && !defined(TMATH_USE_F16C) && defined(__F16C__) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_F16C
#endif

// MSVC平台没有F16C开关，需要开启AVX2
#if defined(_MSC_VER) && !defined(TMATH_USE_F16C) && defined(TMATH_USE_AVX2) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_F16C
#endif

// AVX也需要依赖编译开关: MSVC: /arch:AVX, other: -mavx
// 当然，如果开启了AVX2，肯定就支持AVX
#if !defined(TMATH_USE_AVX) && ( defined(TMATH_USE_AVX2) || defined(__AVX__) ) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_AVX
#endif

// 非MSVC平台需要手动开启SSE4.1，当然如果支持AVX，也就支持SSE4.1了
#if !defined(TMATH_USE_SSE4_1) && ( defined(__SSE4_1__) || defined(TMATH_USE_AVX) ) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_SSE4_1
#endif

// 非MSVC平台需要手动开启SSE3，当然如果支持SSE4.1，也就支持SSE3了
#if !defined(TMATH_USE_SSE3) && ( defined(TMATH_USE_SSE4_1) || defined(__SSE3__) ) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_SSE3
#endif

// 非MSVC平台需要手动开启SSE2，当然如果支持SSE3，也就支持SSE2了
#if !defined(TMATH_USE_SSE2) && ( defined(TMATH_USE_SSE3) || defined(__SSE2__) ) && !defined(TMATH_NO_SIMD)
    #define TMATH_USE_SSE2
#endif

// X86_64平台默认启用SSE2, ARM平台默认启用NEON
#if !defined(TMATH_USE_ARM_NEON) && !defined(TMATH_USE_SSE2) && !defined(TMATH_NO_SIMD)
    #if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC)
        #define TMATH_USE_SSE2
    #elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __arm__ || __aarch64__
        #define TMATH_USE_ARM_NEON
    #elif !defined(TMATH_NO_SIMD)
        #error "tSimd does not support this target"
    #endif
#endif // !TMATH_USE_ARM_NEON && !TMATH_USE_SSE2 && !TMATH_NO_SIMD

// Intel SVML 数学运算指令集
#if defined(TMATH_USE_SSE2) && defined(_MSC_VER) && (_MSC_VER >= 1920) && !defined(__clang__) && !defined(TMATH_USE_SVML) && !defined(TMATH_DISABLE_SVML)
    #define TMATH_USE_SVML
#endif


// 用于测试是否满足指令集依赖关系
#if defined(TMATH_USE_AVX2) && !defined(TMATH_USE_AVX)
    #error "AVX2 requires AVX"
#endif

#if defined(TMATH_USE_AVX) && !defined(TMATH_USE_SSE4_1)
    #error "AVX requires SSE4.1"
#endif

#if defined(TMATH_USE_SSE4_1) && !defined(TMATH_USE_SSE3)
    #error "SSE4.1 requires SSE3"
#endif

#if defined(TMATH_USE_SSE3) && !defined(TMATH_USE_SSE2)
    #error "SSE3 requires SSE2"
#endif

#if defined(TMATH_USE_FMA3) && defined(_MSC_VER) && !defined(TMATH_USE_AVX2)
    #error "FMA3 requires AVX2 (MSVC)"
#endif

#if defined(TMATH_USE_F16C) && defined(_MSC_VER) && !defined(TMATH_USE_AVX2)
    #error "F16C requires AVX2 (MSVC)"
#endif


// SIMD headers
#ifndef TMATH_NO_SIMD

    // SSE2 headers
    #ifdef TMATH_USE_SSE2
        #include <xmmintrin.h>
        #include <emmintrin.h>

        // SSE3 header
        #ifdef TMATH_USE_SSE3
            #include <pmmintrin.h>
        #endif

        // SSE4.1 header
        #ifdef TMATH_USE_SSE4_1
            #include <smmintrin.h>
        #endif

        // AVX header
        #ifdef TMATH_USE_AVX
            #include <immintrin.h>
        #endif

    // ARM NEON header
    #elif defined(TMATH_USE_ARM_NEON)
        #if defined(_MSC_VER) && !defined(__clang__) && (defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC))
            #include <arm64_neon.h>
        #else
            #include <arm_neon.h>
        #endif
    #endif
#endif



// SIMD constants
TMATH_SIMD_NAMESPACE_BEGIN
#define TMATH_DEFINE_SIMD_CONSTEXPR(name, value) static constexpr bool name = value

#if defined(TMATH_USE_SSE2)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE2, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE2, false);
#endif

#if defined(TMATH_USE_SSE3)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE3, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE3, false);
#endif

#if defined(TMATH_USE_SSE4_1)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE4_1, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SSE4_1, false);
#endif

#if defined(TMATH_USE_AVX)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_AVX, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_AVX, false);
#endif

#if defined(TMATH_USE_AVX2)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_AVX2, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_AVX2, false);
#endif

#if defined(TMATH_USE_FMA3)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_FMA3, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_FMA3, false);
#endif

#if defined(TMATH_USE_F16C)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_F16C, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_F16C, false);
#endif

#if defined(TMATH_USE_SVML)
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SVML, true);
#else
    TMATH_DEFINE_SIMD_CONSTEXPR(use_SVML, false);
#endif

TMATH_SIMD_NAMESPACE_END


// checker

#include <initializer_list>


// 如果这个宏被定义，就代表库的代码已经被包含
// 对于测试，我们严格规定，测试代码必须在库代码之后，所以测试代码开头一定要检查这个宏有没有定义，如果没有定义，则给出编译报错
#define TMATH_CHECK_TSIMD_INCLUDE


TMATH_SIMD_NAMESPACE_BEGIN

// 用于测试宏是否开关正确
// 测试opened数组是否全部为true，closed数组是否全部为false
constexpr bool test_simd_intrinsics(std::initializer_list<bool> opened, std::initializer_list<bool> closed)
{
    for (bool b : opened)
    {
        if (!b)
        {
            return false;
        }
    }
    for (bool b : closed)
    {
        if (b)
        {
            return false;
        }
    }
    return true;
}

// usage for testing

// no SIMD
// static_assert(test_simd_intrinsics({}, {use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));

// test SSE2
// static_assert(test_simd_intrinsics({use_SSE2}, {use_SSE3, use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));

// teste SSE3
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3}, {use_SSE4_1, use_AVX, use_AVX2, use_FMA3, use_F16C}));

// teste SSE4_1
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1}, {use_AVX, use_AVX2, use_FMA3, use_F16C}));

// teste AVX
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX}, {use_AVX2, use_FMA3, use_F16C}));

// teste FMA3
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_FMA3}, {use_AVX2, use_F16C}));

// teste F16C
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_F16C}, {use_AVX2, use_FMA3}));

// teste AVX2
// static_assert(test_simd_intrinsics({use_SSE2, use_SSE3, use_SSE4_1, use_AVX, use_F16C, use_FMA3, use_AVX2}, {}));

#include <intrin.h>

struct IntrinsicsCheckResult
{
    bool SSE2   = false;
    bool SSE3   = false;
    bool SSE4_1 = false;
    bool AVX    = false;
    bool AVX2   = false;
    bool F16C   = false;
    bool FMA3   = false;
    bool SVML   = false;
};

inline IntrinsicsCheckResult runtime_check_simd_intrinsics() noexcept
{
    IntrinsicsCheckResult result{};

    int regs[4] = { -1 };

    // ===== CPUID.(EAX=1) =====
    __cpuidex(regs, 1, 0);

    const bool has_sse2   = (regs[3] & (1 << 26)) != 0;
    const bool has_sse3   = (regs[2] & (1 <<  0)) != 0;
    const bool has_sse41  = (regs[2] & (1 << 19)) != 0;
    const bool has_avx    = (regs[2] & (1 << 28)) != 0;
    const bool has_fma    = (regs[2] & (1 << 12)) != 0;
    const bool has_f16c   = (regs[2] & (1 << 29)) != 0;
    const bool has_osxsave= (regs[2] & (1 << 27)) != 0;

    result.SSE2   = has_sse2;
    result.SSE3   = has_sse3;
    result.SSE4_1 = has_sse41;

    // ===== OS 是否支持 AVX 状态保存 =====
    bool os_supports_avx = false;
    if (has_osxsave)
    {
        uint64_t xcr0 = _xgetbv(0);
        // XMM (bit 1) + YMM (bit 2)
        os_supports_avx = (xcr0 & 0x6) == 0x6;
    }

    if (has_avx && os_supports_avx)
    {
        result.AVX  = true;
        result.FMA3 = has_fma;
        result.F16C = has_f16c;
    }

    // ===== CPUID.(EAX=7, ECX=0) =====
    __cpuidex(regs, 7, 0);
    const bool has_avx2 = (regs[1] & (1 << 5)) != 0;

    if (has_avx2 && os_supports_avx)
    {
        result.AVX2 = true;
    }

    // ===== SVML（只能通过编译器判断）=====
#if defined(TMATH_USE_SVML)
    result.SVML = true;
#else
    result.SVML = false;
#endif

    return result;
}

TMATH_SIMD_NAMESPACE_END
