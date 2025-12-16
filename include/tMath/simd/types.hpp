#pragma once

#include <cstdint>

#include "math_defs_simd.hpp"
#include "platform.hpp"

TMATH_SIMD_NAMESPACE_BEGIN

#if defined(TMATH_HAS_SSE2) && !defined(TMATH_NO_SIMD)
using float4 = __m128;
#endif


namespace detail
{
    template<typename T>
        requires std::is_arithmetic_v<T>
    struct alignas(16) TVector4
    {
        union
        {
            T data[4];
            float4 v;
        };

        explicit TVector4(T x, T y, T z, T w)
        {
            data[0] = x;
            data[1] = y;
            data[2] = z;
            data[3] = w;
        }
    };
}

namespace Mask
{
    static const float4 Lane0 = detail::TVector4<uint32_t>(0xffffffff, 0x0, 0x0, 0x0).v;
    static const float4 Lane1 = detail::TVector4<uint32_t>(0x0, 0xffffffff, 0x0, 0x0).v;
    static const float4 Lane2 = detail::TVector4<uint32_t>(0x0, 0x0, 0xffffffff, 0x0).v;
    static const float4 Lane3 = detail::TVector4<uint32_t>(0x0, 0x0, 0x0, 0xffffffff).v;
    static const float4 Lane01 = detail::TVector4<uint32_t>(0xffffffff, 0xffffffff, 0x0, 0x0).v;
    static const float4 Lane012 = detail::TVector4<uint32_t>(0xffffffff, 0xffffffff, 0xffffffff, 0x0).v;
}


TMATH_SIMD_NAMESPACE_END
