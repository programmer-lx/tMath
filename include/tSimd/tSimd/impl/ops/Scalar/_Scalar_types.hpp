#pragma once

#include "../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

namespace Scalar
{
    template<typename scalar_type>
    struct Batch;

    template<>
    struct Batch<float32>
    {
        float32 v;
    };

    template<>
    struct Batch<float64>
    {
        float64 v;
    };
}

TSIMD_NAMESPACE_END
