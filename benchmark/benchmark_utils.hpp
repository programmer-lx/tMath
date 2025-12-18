#pragma once

#include <benchmark/benchmark.h>

#define TMATH_BM_STR_CAT_INNER(a, b) a##b
#define TMATH_BM_STR_CAT(a, b) TMATH_BM_STR_CAT_INNER(a, b)


#ifndef TMATH_BM_REPETITIONS
    #define TMATH_BM_REPETITIONS 5
#endif


#define TMATH_BENCHMARK_MAIN() \
    int main(int argc, char** argv) \
    { \
        tmath_benchmark_prepare(); \
        benchmark::MaybeReenterWithoutASLR(argc, argv); \
        char arg0_default[] = "benchmark"; \
        char* args_default = reinterpret_cast<char*>(arg0_default); \
        if (!argv) \
        { \
            argc = 1; \
            argv = &args_default; \
        } \
        ::benchmark::Initialize(&argc, argv); \
        if (::benchmark::ReportUnrecognizedArguments(argc, argv)) \
            return 1; \
        ::benchmark::RunSpecifiedBenchmarks(); \
        ::benchmark::Shutdown(); \
        return 0; \
    } \
    int main(int, char**)

#define TMATH_BENCHMARK(bm_name, lib_name, exp) \
    static void TMATH_BM_STR_CAT(_TMATH_BM_FN_, __LINE__) (benchmark::State& state) \
    { \
        for (auto _ : state) \
        { \
            exp /* test expression */ \
        } \
    } \
    BENCHMARK(TMATH_BM_STR_CAT(_TMATH_BM_FN_, __LINE__))->Name(std::string(bm_name) + "/" + lib_name)->Repetitions(TMATH_BM_REPETITIONS)->Unit(benchmark::kNanosecond)
