#pragma once

#include <xmmintrin.h> // SSE
#include <emmintrin.h> // SSE2
#include <pmmintrin.h> // SSE3
#include <smmintrin.h> // SSE4.1

#include "../_SSE_family_types.hpp"
#include "../../../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

namespace SSE_family
{
    template<>
    struct Batch<float32>
    {
        __m128 v;
    };
}

TSIMD_NAMESPACE_END