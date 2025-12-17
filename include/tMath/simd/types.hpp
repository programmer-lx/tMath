#pragma once

#include <cstdint>

// #define TMATH_USE_SSE2 // test
// #define TMATH_USE_SSE3 // test
// #define TMATH_NO_SIMD // test
// #define TMATH_USE_FMA3 // test
#if defined(TMATH_NO_SIMD)
    #include "../vector1.hpp"
    #include "../vector2.hpp"
    #include "../vector3.hpp"
    #include "../vector4.hpp"
    #include "../matrix3x3.hpp"
    #include "../matrix4x4.hpp"
#endif

#include "math_defs_simd.hpp"
#include "platform.hpp"

TMATH_SIMD_NAMESPACE_BEGIN

// 128 bits
#if defined(TMATH_NO_SIMD)

    struct float32_4
    {
        TMATH_VECTOR4(float32_4, float)
    };
    using float32_4_arg_in = const float32_4&;

    struct int32_4
    {
        TMATH_VECTOR4(int32_4, int32_t)
    };
    using int32_4_arg_in = const int32_4&;

#elif defined(TMATH_USE_SSE2)

    using float32_4 = __m128;
    using float32_4_arg_in = float32_4;

    using int32_4 = __m128i;
    using int32_4_arg_in = int32_4;

#endif


struct alignas(16) _128bits_mem_block
{
    union
    {
        uint32_t u32_data[4];
        float32_4 f32_4;
        int32_4 i32_4;
    };

    explicit constexpr _128bits_mem_block(uint32_t lane0, uint32_t lane1, uint32_t lane2, uint32_t lane3) noexcept
    {
        u32_data[0] = lane0;
        u32_data[1] = lane1;
        u32_data[2] = lane2;
        u32_data[3] = lane3;
    }
};

namespace Mask128
{
    // 四个数，符号位是0，其余位是1
    static constexpr _128bits_mem_block Abs4 = _128bits_mem_block(0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff);

    static constexpr _128bits_mem_block Lane0 = _128bits_mem_block(0xffffffff, 0x0, 0x0, 0x0);
    static constexpr _128bits_mem_block Lane1 = _128bits_mem_block(0x0, 0xffffffff, 0x0, 0x0);
    static constexpr _128bits_mem_block Lane2 = _128bits_mem_block(0x0, 0x0, 0xffffffff, 0x0);
    static constexpr _128bits_mem_block Lane3 = _128bits_mem_block(0x0, 0x0, 0x0, 0xffffffff);
    static constexpr _128bits_mem_block Lane01 = _128bits_mem_block(0xffffffff, 0xffffffff, 0x0, 0x0);
    static constexpr _128bits_mem_block Lane012 = _128bits_mem_block(0xffffffff, 0xffffffff, 0xffffffff, 0x0);
}


TMATH_SIMD_NAMESPACE_END
