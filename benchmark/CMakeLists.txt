function(action_of_benchmark_test_target TARGET_NAME)
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:/Ox>)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:-O3>)
    endif()
    target_compile_definitions(${TARGET_NAME} PRIVATE TMATH_IS_DOING_BENCHMARK TMATH_IS_TESTING)
    target_link_libraries(${TARGET_NAME} PRIVATE tMath benchmark::benchmark benchmark::benchmark_main)
endfunction()

if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(UNICODE _UNICODE)
endif()

# NO SIMD
add_executable(benchmark_NO_SIMD NO_SIMD.cpp)
action_of_benchmark_test_target(benchmark_NO_SIMD)

# SSE2
add_executable(benchmark_simd_SSE2 SSE2.cpp)
action_of_benchmark_test_target(benchmark_simd_SSE2)
if(MSVC)
#    target_compile_options(benchmark_simd_SSE2 PRIVATE /arch:SSE2) # /arch:SSE2 被编译器忽略
else()
    target_compile_options(benchmark_simd_SSE2 PRIVATE -msse2)
endif()
get_target_property(OPTIONS_simd_SSE2 benchmark_simd_SSE2 COMPILE_OPTIONS)
message(STATUS "benchmark_simd_SSE2 compile options: ${OPTIONS_simd_SSE2}")

# SSE3
add_executable(benchmark_simd_SSE3 SSE3.cpp)
action_of_benchmark_test_target(benchmark_simd_SSE3)
if(MSVC)
    target_compile_options(benchmark_simd_SSE3 PRIVATE /arch:AVX) # 官方文档说可以使用SSE4.2，但是实测无效
else()
    target_compile_options(benchmark_simd_SSE3 PRIVATE -msse3)
endif()
get_target_property(OPTIONS_simd_SSE3 benchmark_simd_SSE3 COMPILE_OPTIONS)
message(STATUS "benchmark_simd_SSE3 compile options: ${OPTIONS_simd_SSE3}")

# SSE4.1
add_executable(benchmark_simd_SSE4_1 SSE4_1.cpp)
action_of_benchmark_test_target(benchmark_simd_SSE4_1)
if(MSVC)
    target_compile_options(benchmark_simd_SSE4_1 PRIVATE /arch:AVX) # 官方文档说可以使用SSE4.2，但是实测无效
else()
    target_compile_options(benchmark_simd_SSE4_1 PRIVATE -msse4.1)
endif()

# AVX
add_executable(benchmark_simd_AVX AVX.cpp)
action_of_benchmark_test_target(benchmark_simd_AVX)
if(MSVC)
    target_compile_options(benchmark_simd_AVX PRIVATE /arch:AVX)
else()
    target_compile_options(benchmark_simd_AVX PRIVATE -mavx)
endif()
get_target_property(OPTIONS_simd_AVX benchmark_simd_AVX COMPILE_OPTIONS)
message(STATUS "benchmark_simd_AVX compile options: ${OPTIONS_simd_AVX}")

# FMA3
add_executable(benchmark_simd_FMA3 FMA3.cpp)
action_of_benchmark_test_target(benchmark_simd_FMA3)
if(MSVC)
    target_compile_options(benchmark_simd_FMA3 PRIVATE /arch:AVX2)
else()
    target_compile_options(benchmark_simd_FMA3 PRIVATE -mfma)
endif()

# F16C
add_executable(benchmark_simd_F16C F16C.cpp)
action_of_benchmark_test_target(benchmark_simd_F16C)
if(MSVC)
    target_compile_options(benchmark_simd_F16C PRIVATE /arch:AVX2)
else()
    target_compile_options(benchmark_simd_F16C PRIVATE -mf16c)
endif()

# AVX2
add_executable(benchmark_simd_AVX2 AVX2.cpp)
action_of_benchmark_test_target(benchmark_simd_AVX2)
if(MSVC)
    target_compile_options(benchmark_simd_AVX2 PRIVATE /arch:AVX2)
else()
    target_compile_options(benchmark_simd_AVX2 PRIVATE -mavx2)
endif()

# SVML
if(MSVC)
    add_executable(benchmark_simd_SVML SVML.cpp)
    action_of_benchmark_test_target(benchmark_simd_SVML)
endif()


set(TMATH_BENCHMARK_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/benchmarks/bin)
foreach(tgt IN LISTS TMATH_BENCHMARK_TARGETS)
    set_target_properties(${tgt} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${TMATH_BENCHMARK_BIN_DIR}
    )
endforeach()


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

set(TMATH_BM_JSON_OUTPUT_DIR ${TMATH_WEBSITE_DIR}/benchmark_data/${TMATH_BENCHMARK_DATE}/${TMATH_BENCHMARK_OS})
file(MAKE_DIRECTORY ${TMATH_BM_JSON_OUTPUT_DIR})

set(TMATH_BM_DATES_JSON_PATH ${TMATH_WEBSITE_DIR}/benchmark_data/dates.json)

function(add_simd_benchmark_test simd_name target)
    add_test(
            NAME Benchmark_${simd_name}
            COMMAND
            python
            ${TMATH_SCRIPTS_DIR}/run_benchmark_and_minimize.py
            $<TARGET_FILE:${target}>
            ${TMATH_BM_JSON_OUTPUT_DIR}/${simd_name}.json
            ${TMATH_BM_DATES_JSON_PATH}
            ${TMATH_BENCHMARK_DATE}
            $<TARGET_FILE:minimize_bm_json>
    )
endfunction()

# 使用test工具来输出benchmark结果，并非真的在测试
add_simd_benchmark_test(NO_SIMD benchmark_NO_SIMD)
add_simd_benchmark_test(SSE2    benchmark_simd_SSE2)
add_simd_benchmark_test(SSE3    benchmark_simd_SSE3)
add_simd_benchmark_test(SSE4_1  benchmark_simd_SSE4_1)
#add_simd_benchmark_test(AVX     benchmark_simd_AVX)
add_simd_benchmark_test(FMA3    benchmark_simd_FMA3)
#add_simd_benchmark_test(F16C    benchmark_simd_F16C)
#add_simd_benchmark_test(AVX2    benchmark_simd_AVX2)
if(MSVC)
    add_simd_benchmark_test(SVML benchmark_simd_SVML)
endif()