#pragma once

#include <cstdint>

#include "math_defs_simd.hpp"
#include "platform.hpp"

TMATH_SIMD_NAMESPACE_BEGIN

// 128 bits
#if defined(TMATH_USE_SSE2) && !defined(TMATH_NO_SIMD)
    using float32_4 = __m128;
    using int32_4 = __m128i;
#endif

// 256 bits
#if defined(TMATH_USE_AVX)
    using float64_4 = __m256;
    using int64_4 = __m256i;
#else
    using float64_4 = __m128[2];
    using int64_4 = __m128i[2];
#endif


struct alignas(alignof(float32_4)) _128bits_mem_block
{
    union
    {
        uint32_t u32_data[4];
        float32_4 f32_4;
        int32_4 i32_4;
    };

    explicit constexpr _128bits_mem_block(uint32_t lane0, uint32_t lane1, uint32_t lane2, uint32_t lane3)
    {
        u32_data[0] = lane0;
        u32_data[1] = lane1;
        u32_data[2] = lane2;
        u32_data[3] = lane3;
    }
};

struct alignas(alignof(float64_4)) _256bits_mem_block
{
    union
    {
        uint64_t u64_data[4];
        float64_4 f64_4;
        int64_4 i64_4;
    };

    explicit constexpr _256bits_mem_block(uint32_t lane0, uint32_t lane1, uint32_t lane2, uint32_t lane3)
    {
        u64_data[0] = lane0;
        u64_data[1] = lane1;
        u64_data[2] = lane2;
        u64_data[3] = lane3;
    }
};

namespace Mask128
{
    static constexpr _128bits_mem_block Lane0 = _128bits_mem_block(0xffffffff, 0x0, 0x0, 0x0);
    static constexpr _128bits_mem_block Lane1 = _128bits_mem_block(0x0, 0xffffffff, 0x0, 0x0);
    static constexpr _128bits_mem_block Lane2 = _128bits_mem_block(0x0, 0x0, 0xffffffff, 0x0);
    static constexpr _128bits_mem_block Lane3 = _128bits_mem_block(0x0, 0x0, 0x0, 0xffffffff);
    static constexpr _128bits_mem_block Lane01 = _128bits_mem_block(0xffffffff, 0xffffffff, 0x0, 0x0);
    static constexpr _128bits_mem_block Lane012 = _128bits_mem_block(0xffffffff, 0xffffffff, 0xffffffff, 0x0);
}


TMATH_SIMD_NAMESPACE_END
