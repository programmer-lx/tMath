#pragma once

#include <immintrin.h> // AVX

#include "../../dispatch.hpp"

TSIMD_NAMESPACE_BEGIN

// AVX512f默认支持 FMA 运算
template<>
struct SimdOp<SimdInstruction::AVX512_F, float>
{
    TSIMD_DETAIL_SIMD_OP_TRAITS_AND_CONSTANTS(AVX512_F, float, __m512, Alignment::AVX512_Family)

    TSIMD_OP_AVX512_F_API static batch_t TSIMD_CALL_CONV TSIMD_op_load(const float* mem) noexcept
    {
        return _mm512_load_ps(mem);
    }

    TSIMD_OP_AVX512_F_API static batch_t TSIMD_CALL_CONV TSIMD_op_loadu(const float* mem) noexcept
    {
        return _mm512_loadu_ps(mem);
    }

    TSIMD_OP_AVX512_F_API static void TSIMD_CALL_CONV TSIMD_op_store(float* mem, batch_t v) noexcept
    {
        _mm512_store_ps(mem, v);
    }

    TSIMD_OP_AVX512_F_API static void TSIMD_CALL_CONV TSIMD_op_storeu(float* mem, batch_t v) noexcept
    {
        _mm512_storeu_ps(mem, v);
    }

    TSIMD_OP_AVX512_F_API static batch_t TSIMD_CALL_CONV TSIMD_op_zero() noexcept
    {
        return _mm512_setzero_ps();
    }

    TSIMD_OP_AVX512_F_API static batch_t TSIMD_CALL_CONV TSIMD_op_set(float x) noexcept
    {
        return _mm512_set1_ps(x);
    }

    TSIMD_OP_AVX512_F_API static batch_t TSIMD_CALL_CONV TSIMD_op_add(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm512_add_ps(lhs, rhs);
    }

    TSIMD_OP_AVX512_F_API static batch_t TSIMD_CALL_CONV TSIMD_op_sub(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm512_sub_ps(lhs, rhs);
    }

    TSIMD_OP_AVX512_F_API static batch_t TSIMD_CALL_CONV TSIMD_op_mul(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm512_mul_ps(lhs, rhs);
    }

    TSIMD_OP_AVX512_F_API static batch_t TSIMD_CALL_CONV TSIMD_op_div(batch_t lhs, batch_t rhs) noexcept
    {
        return _mm512_div_ps(lhs, rhs);
    }

    TSIMD_OP_AVX512_F_API static float TSIMD_CALL_CONV TSIMD_op_reduce_sum(batch_t v) noexcept
    {
        return _mm512_reduce_add_ps(v);
    }

    TSIMD_OP_AVX512_F_API static batch_t TSIMD_CALL_CONV TSIMD_op_mul_add(batch_t a, batch_t b, batch_t c) noexcept
    {
        return _mm512_fmadd_ps(a, b, c);
    }
};

TSIMD_NAMESPACE_END
