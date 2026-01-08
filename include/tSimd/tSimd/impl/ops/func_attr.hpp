#pragma once

#include "../platform.hpp"

// see https://gcc.gnu.org/onlinedocs/gcc/x86-Function-Attributes.html#x86-Function-Attributes for more intrinsics attributes information

// scalar
#define TSIMD_SCALAR_INTRINSIC_ATTR
#define TSIMD_OP_SCALAR_API \
    TMATH_FORCE_INLINE \
    TMATH_FLATTEN \
    TSIMD_SCALAR_INTRINSIC_ATTR


// sse
#define TSIMD_SSE_INTRINSIC_ATTR TMATH_FUNC_ATTR_INTRINSIC_TARGETS("sse")
#define TSIMD_OP_SSE_API \
    TMATH_FORCE_INLINE \
    TMATH_FLATTEN \
    TSIMD_SSE_INTRINSIC_ATTR


// sse2
#define TSIMD_SSE2_INTRINSIC_ATTR TMATH_FUNC_ATTR_INTRINSIC_TARGETS("sse2")
#define TSIMD_OP_SSE2_API \
    TMATH_FORCE_INLINE \
    TMATH_FLATTEN \
    TSIMD_SSE2_INTRINSIC_ATTR



// sse3
#define TSIMD_SSE3_INTRINSIC_ATTR TMATH_FUNC_ATTR_INTRINSIC_TARGETS("sse3")
#define TSIMD_OP_SSE3_API \
    TMATH_FORCE_INLINE \
    TMATH_FLATTEN \
    TSIMD_SSE3_INTRINSIC_ATTR


// sse4.1
#define TSIMD_SSE4_1_INTRINSIC_ATTR TMATH_FUNC_ATTR_INTRINSIC_TARGETS("sse4.1")
#define TSIMD_OP_SSE4_1_API \
    TMATH_FORCE_INLINE \
    TMATH_FLATTEN \
    TSIMD_SSE4_1_INTRINSIC_ATTR


// sse4.2
#define TSIMD_SSE4_2_INTRINSIC_ATTR TMATH_FUNC_ATTR_INTRINSIC_TARGETS("sse4.2")
#define TSIMD_OP_SSE4_2_API \
    TMATH_FORCE_INLINE \
    TMATH_FLATTEN \
    TSIMD_SSE4_2_INTRINSIC_ATTR



// avx(no fma3)
#define TSIMD_AVX_INTRINSIC_ATTR TMATH_FUNC_ATTR_INTRINSIC_TARGETS("avx")
#define TSIMD_OP_AVX_API \
    TMATH_FORCE_INLINE \
    TMATH_FLATTEN \
    TSIMD_AVX_INTRINSIC_ATTR


// avx2
#define TSIMD_AVX2_INTRINSIC_ATTR TMATH_FUNC_ATTR_INTRINSIC_TARGETS("avx2")
#define TSIMD_OP_AVX2_API \
    TMATH_FORCE_INLINE \
    TMATH_FLATTEN \
    TSIMD_AVX2_INTRINSIC_ATTR


// avx2+fma3
#define TSIMD_AVX2_FMA3_INTRINSIC_ATTR TMATH_FUNC_ATTR_INTRINSIC_TARGETS("avx2,fma")
#define TSIMD_OP_AVX2_FMA3_API \
    TMATH_FORCE_INLINE \
    TMATH_FLATTEN \
    TSIMD_AVX2_FMA3_INTRINSIC_ATTR


// func sig
#define TSIMD_OP_SIG_SCALAR(ret, func_name, params)     TSIMD_OP_SCALAR_API     static ret TSIMD_SCALAR_CALL_CONV   func_name params noexcept

#define TSIMD_OP_SIG_SSE(ret, func_name, params)        TSIMD_OP_SSE_API        static ret TSIMD_CALL_CONV          func_name params noexcept
#define TSIMD_OP_SIG_SSE2(ret, func_name, params)       TSIMD_OP_SSE2_API       static ret TSIMD_CALL_CONV          func_name params noexcept
#define TSIMD_OP_SIG_SSE3(ret, func_name, params)       TSIMD_OP_SSE3_API       static ret TSIMD_CALL_CONV          func_name params noexcept
#define TSIMD_OP_SIG_SSE4_1(ret, func_name, params)     TSIMD_OP_SSE4_1_API     static ret TSIMD_CALL_CONV          func_name params noexcept
#define TSIMD_OP_SIG_SSE4_2(ret, func_name, params)     TSIMD_OP_SSE4_2_API     static ret TSIMD_CALL_CONV          func_name params noexcept

#define TSIMD_OP_SIG_AVX(ret, func_name, params)        TSIMD_OP_AVX_API        static ret TSIMD_CALL_CONV          func_name params noexcept
#define TSIMD_OP_SIG_AVX2(ret, func_name, params)       TSIMD_OP_AVX2_API       static ret TSIMD_CALL_CONV          func_name params noexcept
#define TSIMD_OP_SIG_AVX2_FMA3(ret, func_name, params)  TSIMD_OP_AVX2_FMA3_API  static ret TSIMD_CALL_CONV          func_name params noexcept
