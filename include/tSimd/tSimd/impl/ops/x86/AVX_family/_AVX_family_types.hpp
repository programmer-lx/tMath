#pragma once

#include <immintrin.h> // AVX

#include "../../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

namespace AVX_family
{
    template<typename scalar_type>
    struct Batch;

    template<>
    struct Batch<float32>
    {
        __m256 v;
    };

    template<>
    struct Batch<float64>
    {
        __m256d v;
    };
}

TSIMD_NAMESPACE_END
