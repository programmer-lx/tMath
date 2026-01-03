#pragma once

#include <thread>
#include <random>

#include <benchmark/benchmark.h>

#define TMATH_BM_STR_CAT_INNER(a, b) a##b
#define TMATH_BM_STR_CAT(a, b) TMATH_BM_STR_CAT_INNER(a, b)


#ifndef TMATH_BM_REPETITIONS
    #define TMATH_BM_REPETITIONS 5
#endif

#ifndef TMATH_BM_ITERATIONS
    #define TMATH_BM_ITERATIONS 1e5
#endif

constexpr int RandomFloatCount = 64;
static float g_random_floats[RandomFloatCount];
static int cur_random_float_index = 0;
static float next_random_float()
{
    cur_random_float_index = (cur_random_float_index + 1) % RandomFloatCount;
    return g_random_floats[cur_random_float_index];
}
static void setup_random_floats(const benchmark::State& state)
{
    std::mt19937 rng(12345); // 固定种子
    std::uniform_real_distribution<float> dist(-100.f, 100.f);

    for (size_t i = 0; i < RandomFloatCount; ++i)
    {
        g_random_floats[i] = dist(rng);
    }
}

constexpr int Random_float32_4_Count = 64;
static tsimd::float32_4 g_random_float32_4s[Random_float32_4_Count];
static int cur_random_float32_4_index = 0;
static tsimd::float32_4 next_random_float32_4()
{
    cur_random_float32_4_index = (cur_random_float32_4_index + 1) % RandomFloatCount;
    return g_random_float32_4s[cur_random_float32_4_index];
}
static void setup_random_float32_4s(const benchmark::State& state)
{
    std::mt19937 rng(12345); // 固定种子
    std::uniform_real_distribution<float> dist(-100.f, 100.f);

    for (size_t i = 0; i < RandomFloatCount; ++i)
    {
        g_random_float32_4s[i] = tsimd::load(dist(rng), dist(rng), dist(rng), dist(rng));
    }
}


#define TMATH_BENCHMARK(fn_sig, comment, op_count, ...) \
    static void TMATH_BM_STR_CAT(_TMATH_BM_FN_, __LINE__) (benchmark::State& state) \
    { \
        for (auto _ : state) \
        { \
            for (int TMATH_op_count___= 0; TMATH_op_count___ < op_count; ++TMATH_op_count___) { \
                __VA_ARGS__ /* test expression */ \
            } \
        } \
    } \
    BENCHMARK(TMATH_BM_STR_CAT(_TMATH_BM_FN_, __LINE__))\
    ->Name(std::string(fn_sig) + "/" + comment + "/" + #op_count)\
    ->Repetitions(TMATH_BM_REPETITIONS)\
    ->Iterations(TMATH_BM_ITERATIONS)\
    ->Unit(benchmark::kNanosecond)\
    ->ReportAggregatesOnly(true)
