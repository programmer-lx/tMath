#pragma once

#include <immintrin.h> // AVX

#include "../_AVX_family_types.hpp"
#include "../../../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

namespace AVX_family
{
    template<>
    struct Batch<float32>
    {
        __m256 v;
    };
}

TSIMD_NAMESPACE_END
