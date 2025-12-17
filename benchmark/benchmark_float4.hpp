#pragma once

#include <stdexcept>
#include <iostream>

#include <benchmark/benchmark.h>

#include "benchmark_types.hpp"
#include "../tests/test.hpp"
#include <tMath/simd/float32_4.hpp>

using namespace tSimd;

static std::vector<float> g_data_a(1024 * 4, 1.1f);
static std::vector<float> g_data_b(1024 * 4, 2.2f);

// --- 1. 纯标量版本 ---
static void BM_Dot4_Scalar_Correct(benchmark::State& state)
{
    for (auto _ : state) {
        float total = 0.0f;
        // 强制循环 1024 次，与 SIMD 版本完全对等
        for (size_t i = 0; i < 1024; ++i) {
            size_t idx = i * 4;
            // 模拟 AoS (Array of Structures) 访问
            float d = g_data_a[idx+0] * g_data_b[idx+0] +
                      g_data_a[idx+1] * g_data_b[idx+1] +
                      g_data_a[idx+2] * g_data_b[idx+2] +
                      g_data_a[idx+3] * g_data_b[idx+3];
            total += d;
        }
        benchmark::DoNotOptimize(total);
    }
    // 设置处理总量，这样结果里的 Time 这一列代表处理 1024 个向量的总时间
    state.SetItemsProcessed(state.iterations() * 1024);
}
BENCHMARK(BM_Dot4_Scalar_Correct);

static void BM_Dot4_SIMD_Unrolled(benchmark::State& state)
{
    const float32_4* a_ptr = reinterpret_cast<const float32_4*>(g_data_a.data());
    const float32_4* b_ptr = reinterpret_cast<const float32_4*>(g_data_b.data());

    for (auto _ : state) {
        float32_4 acc0 = tSimd::set(0.0f);
        float32_4 acc1 = tSimd::set(0.0f);
        float32_4 acc2 = tSimd::set(0.0f);
        float32_4 acc3 = tSimd::set(0.0f);

        for (size_t i = 0; i < 1024; i += 4)
        {
            // CPU 会尝试同时发射这 4 条 dot4 指令
            acc0 = tSimd::add(acc0, tSimd::dot4(a_ptr[i+0], b_ptr[i+0]));
            acc1 = tSimd::add(acc1, tSimd::dot4(a_ptr[i+1], b_ptr[i+1]));
            acc2 = tSimd::add(acc2, tSimd::dot4(a_ptr[i+2], b_ptr[i+2]));
            acc3 = tSimd::add(acc3, tSimd::dot4(a_ptr[i+3], b_ptr[i+3]));
        }
        float32_4 final_acc = tSimd::add(tSimd::add(acc0, acc1), tSimd::add(acc2, acc3));
        benchmark::DoNotOptimize(final_acc);
    }
    state.SetItemsProcessed(state.iterations() * 1024);
}
BENCHMARK(BM_Dot4_SIMD_Unrolled);


inline void benchmark_float4()
{
    int argc = 0;
    char** argv = nullptr;

    benchmark::MaybeReenterWithoutASLR(argc, argv);
    char arg0_default[] = "benchmark";
    char* args_default = reinterpret_cast<char*>(arg0_default);
    if (!argv)
    {
        argc = 1;
        argv = &args_default;
    }
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
    {
        throw std::runtime_error("");
    }
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
}
