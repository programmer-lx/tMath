
if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(UNICODE _UNICODE)
endif()

# SSE2
add_executable(benchmark_simd_SSE2 SSE2.cpp)
target_link_libraries(benchmark_simd_SSE2 PRIVATE tMath benchmark::benchmark benchmark::benchmark)
add_o2_optimization(benchmark_simd_SSE2)

# SSE3
add_executable(benchmark_simd_SSE3 SSE3.cpp)
target_link_libraries(benchmark_simd_SSE3 PRIVATE tMath benchmark::benchmark)
add_o2_optimization(benchmark_simd_SSE3)

# SSE4.1
add_executable(benchmark_simd_SSE4_1 SSE4_1.cpp)
target_link_libraries(benchmark_simd_SSE4_1 PRIVATE tMath benchmark::benchmark)
add_o2_optimization(benchmark_simd_SSE4_1)

# AVX
add_executable(benchmark_simd_AVX AVX.cpp)
target_link_libraries(benchmark_simd_AVX PRIVATE tMath benchmark::benchmark)
add_o2_optimization(benchmark_simd_AVX)

# FMA3
add_executable(benchmark_simd_FMA3 FMA3.cpp)
target_link_libraries(benchmark_simd_FMA3 PRIVATE tMath benchmark::benchmark)
add_o2_optimization(benchmark_simd_FMA3)

# F16C
add_executable(benchmark_simd_F16C F16C.cpp)
target_link_libraries(benchmark_simd_F16C PRIVATE tMath benchmark::benchmark)
add_o2_optimization(benchmark_simd_F16C)

# AVX2
add_executable(benchmark_simd_AVX2 AVX2.cpp)
target_link_libraries(benchmark_simd_AVX2 PRIVATE tMath benchmark::benchmark)
add_o2_optimization(benchmark_simd_AVX2)

add_test(NAME Benchmark_simd_SSE2 COMMAND $<TARGET_FILE:benchmark_simd_SSE2>)
add_test(NAME Benchmark_simd_SSE3 COMMAND $<TARGET_FILE:benchmark_simd_SSE3>)
add_test(NAME Benchmark_simd_SSE4_1 COMMAND $<TARGET_FILE:benchmark_simd_SSE4_1>)
add_test(NAME Benchmark_simd_AVX COMMAND $<TARGET_FILE:benchmark_simd_AVX>)
add_test(NAME Benchmark_simd_FMA3 COMMAND $<TARGET_FILE:benchmark_simd_FMA3>)
add_test(NAME Benchmark_simd_F16C COMMAND $<TARGET_FILE:benchmark_simd_F16C>)
add_test(NAME Benchmark_simd_AVX2 COMMAND $<TARGET_FILE:benchmark_simd_AVX2>)
