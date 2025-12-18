function(bm_add_o3_optimization TARGET_NAME)
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:/Ox>)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:-O3>)
    endif()
endfunction()

if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(UNICODE _UNICODE)
endif()

# SSE2
add_executable(benchmark_simd_SSE2 SSE2.cpp)
target_link_libraries(benchmark_simd_SSE2 PRIVATE tMath benchmark::benchmark benchmark::benchmark)
bm_add_o3_optimization(benchmark_simd_SSE2)

# SSE3
add_executable(benchmark_simd_SSE3 SSE3.cpp)
target_link_libraries(benchmark_simd_SSE3 PRIVATE tMath benchmark::benchmark)
bm_add_o3_optimization(benchmark_simd_SSE3)

# SSE4.1
add_executable(benchmark_simd_SSE4_1 SSE4_1.cpp)
target_link_libraries(benchmark_simd_SSE4_1 PRIVATE tMath benchmark::benchmark)
bm_add_o3_optimization(benchmark_simd_SSE4_1)

# AVX
add_executable(benchmark_simd_AVX AVX.cpp)
target_link_libraries(benchmark_simd_AVX PRIVATE tMath benchmark::benchmark)
bm_add_o3_optimization(benchmark_simd_AVX)

# FMA3
add_executable(benchmark_simd_FMA3 FMA3.cpp)
target_link_libraries(benchmark_simd_FMA3 PRIVATE tMath benchmark::benchmark)
bm_add_o3_optimization(benchmark_simd_FMA3)

# F16C
add_executable(benchmark_simd_F16C F16C.cpp)
target_link_libraries(benchmark_simd_F16C PRIVATE tMath benchmark::benchmark)
bm_add_o3_optimization(benchmark_simd_F16C)

# AVX2
add_executable(benchmark_simd_AVX2 AVX2.cpp)
target_link_libraries(benchmark_simd_AVX2 PRIVATE tMath benchmark::benchmark)
bm_add_o3_optimization(benchmark_simd_AVX2)

set(TMATH_TMATH_BENCHMARK_TARGETS
        benchmark_simd_SSE2
        benchmark_simd_SSE3
        benchmark_simd_SSE4_1
        benchmark_simd_AVX
        benchmark_simd_FMA3
        benchmark_simd_F16C
        benchmark_simd_AVX2
)
set(TMATH_BENCHMARK_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/benchmarks/bin)
foreach(tgt IN LISTS TMATH_BENCHMARK_TARGETS)
    set_target_properties(${tgt} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${TMATH_BENCHMARK_BIN_DIR}
    )
endforeach()

set(TMATH_BENCHMARK_JSON_DIR ${CMAKE_CURRENT_BINARY_DIR}/benchmarks/json)
file(MAKE_DIRECTORY ${TMATH_BENCHMARK_JSON_DIR})


# 精简benchmark输出的json文件的工具
add_executable(minimize_bm_json tools/minimize_benchmark_json.cpp)
target_include_directories(minimize_bm_json PRIVATE ${TMATH_JSON_INCLUDE_DIR})
target_sources(minimize_bm_json PRIVATE ${TMATH_JSON_INCLUDE_DIR}/nlohmann/json.hpp)


# YYYY-MM-DD
string(TIMESTAMP TMATH_BENCHMARK_DATE "%Y-%m-%d")

# system name
if(WIN32)
    set(TMATH_BENCHMARK_OS windows)
elseif(APPLE)
    set(TMATH_BENCHMARK_OS macos)
elseif(UNIX)
    set(TMATH_BENCHMARK_OS linux)
else()
    message(FATAL_ERROR "Unsupported OS for benchmark")
endif()

set(TMATH_BM_JSON_OUTPUT_DIR ${TMATH_WEBSITE_DIR}/${TMATH_BENCHMARK_DATE}/${TMATH_BENCHMARK_OS})
file(MAKE_DIRECTORY ${TMATH_BM_JSON_OUTPUT_DIR})

function(add_simd_benchmark_test simd_name target)
    add_test(
            NAME Benchmark_${simd_name}
            COMMAND
            python
            ${TMATH_SCRIPTS_DIR}/run_benchmark_and_minimize.py
            $<TARGET_FILE:${target}>
            ${TMATH_BM_JSON_OUTPUT_DIR}/${simd_name}.json
            $<TARGET_FILE:minimize_bm_json>
    )
endfunction()

# 使用test工具来输出benchmark结果，并非真的在测试
add_simd_benchmark_test(SSE2    benchmark_simd_SSE2)
add_simd_benchmark_test(SSE3    benchmark_simd_SSE3)
add_simd_benchmark_test(SSE4_1  benchmark_simd_SSE4_1)
add_simd_benchmark_test(AVX     benchmark_simd_AVX)
add_simd_benchmark_test(FMA3    benchmark_simd_FMA3)
add_simd_benchmark_test(F16C    benchmark_simd_F16C)
add_simd_benchmark_test(AVX2    benchmark_simd_AVX2)