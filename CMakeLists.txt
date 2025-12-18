cmake_minimum_required(VERSION 3.28)
project(
        tMath
        LANGUAGES CXX
        VERSION 0.0.0.1
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(tMath INTERFACE)
target_include_directories(tMath INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


# options
option(TMATH_BUILD_EXAMPLES "" OFF)
option(TMATH_BUILD_TESTS "" OFF)
option(TMATH_BUILD_BENCHMARKS "" OFF)

if(TMATH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(TMATH_BUILD_TESTS)
    add_compile_definitions(TMATH_IS_TESTING)

    add_subdirectory(3rdparty/googletest)

    if(MSVC)
        add_compile_options(/arch:AVX2)
    else()
        add_compile_options(-mavx2 -mfma -mf16c -msse4.1 -msse3 -msse2)
    endif()

    enable_testing()
    add_subdirectory(tests)
endif()

if(TMATH_BUILD_BENCHMARKS)

    set(TMATH_JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/json/single_include)
    set(TMATH_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
    set(TMATH_WEBSITE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/website)

    add_compile_definitions(TMATH_IS_TESTING)

    set(BENCHMARK_ENABLE_TESTING OFF)
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
    add_subdirectory(3rdparty/benchmark)

    enable_testing()
    add_subdirectory(benchmark)
endif()
