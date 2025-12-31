#pragma once

#include <cstdint>

// #undef TMATH_NO_SIMD // test
// #define TMATH_USE_SSE2 // test
// #define TMATH_USE_SSE3 // test
// #define TMATH_NO_SIMD // test
// #define TMATH_USE_FMA3 // test
// #define TMATH_USE_AVX2 // test
// #define TMATH_DISABLE_SVML // test
#if defined(TMATH_NO_SIMD)
    #include "../number.hpp"
    #include "../vector.hpp"
    #include "../matrix.hpp"
#endif

#include "math_defs_simd.hpp"
#include "platform.hpp"

TMATH_SIMD_NAMESPACE_BEGIN

#define TMATH_SIMD_128_ALIGNMENT 16
#define TMATH_SIMD_256_ALIGNMENT 32

static constexpr size_t _128_alignment = TMATH_SIMD_128_ALIGNMENT;
static constexpr size_t _256_alignment = TMATH_SIMD_256_ALIGNMENT;

// 128 bits
#if defined(TMATH_NO_SIMD)

    struct float32_4
    {
        TMATH_FULL_VECTOR4(float32_4, float)
    };
    using float32_4_arg_in = const float32_4&;

    struct int32_4
    {
        TMATH_FULL_VECTOR4(int32_4, int32_t)
    };
    using int32_4_arg_in = const int32_4&;

#elif defined(TMATH_USE_SSE2)

    using float32_4 = __m128;
    using float32_4_arg_in = float32_4;

    using int32_4 = __m128i;
    using int32_4_arg_in = int32_4;

#endif


struct alignas(TMATH_SIMD_128_ALIGNMENT) _128bits_mem_block
{
    union
    {
        uint32_t u32_data[4];
        float32_4 f32_4;
    };

    static_assert(sizeof(u32_data) == sizeof(float32_4), "sizeof(u32_data) must equal to sizeof(float32_4)");

    static constexpr _128bits_mem_block make_from_uint32(uint32_t lane0, uint32_t lane1, uint32_t lane2, uint32_t lane3) noexcept
    {
        _128bits_mem_block result{};

        result.u32_data[0] = lane0;
        result.u32_data[1] = lane1;
        result.u32_data[2] = lane2;
        result.u32_data[3] = lane3;

        return result;
    }
};

namespace Mask128
{
    static constexpr _128bits_mem_block ZeroBlock = _128bits_mem_block::make_from_uint32(0x0, 0x0, 0x0, 0x0);
    static constexpr _128bits_mem_block OneBlock = _128bits_mem_block::make_from_uint32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);

    // 四个数，符号位是0，其余位是1
    static constexpr _128bits_mem_block Abs4 = _128bits_mem_block::make_from_uint32(0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff);

    static constexpr _128bits_mem_block Lane0 = _128bits_mem_block::make_from_uint32(0xffffffff, 0x0, 0x0, 0x0);
    static constexpr _128bits_mem_block Lane1 = _128bits_mem_block::make_from_uint32(0x0, 0xffffffff, 0x0, 0x0);
    static constexpr _128bits_mem_block Lane2 = _128bits_mem_block::make_from_uint32(0x0, 0x0, 0xffffffff, 0x0);
    static constexpr _128bits_mem_block Lane3 = _128bits_mem_block::make_from_uint32(0x0, 0x0, 0x0, 0xffffffff);
    static constexpr _128bits_mem_block Lane01 = _128bits_mem_block::make_from_uint32(0xffffffff, 0xffffffff, 0x0, 0x0);
    static constexpr _128bits_mem_block Lane012 = _128bits_mem_block::make_from_uint32(0xffffffff, 0xffffffff, 0xffffffff, 0x0);
}


TMATH_SIMD_NAMESPACE_END
