#pragma once

#include <xmmintrin.h> // SSE
#include <emmintrin.h> // SSE2
#include <pmmintrin.h> // SSE3
#include <smmintrin.h> // SSE4.1

#include "../../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

namespace SSE_family
{
    template<typename scalar_type>
    struct Batch;

    template<>
    struct Batch<float32>
    {
        __m128 v;
    };

    template<>
    struct Batch<float64>
    {
        __m128d v;
    };
}

TSIMD_NAMESPACE_END