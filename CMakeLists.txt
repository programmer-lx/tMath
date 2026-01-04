cmake_minimum_required(VERSION 3.22.1)
project(
        tMath
        LANGUAGES CXX
        VERSION 0.0.0.1
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# options
option(TMATH_BUILD_EXAMPLES "" OFF)
option(TMATH_BUILD_TESTS "" OFF)
option(TMATH_BUILD_BENCHMARKS "" OFF)


# tMath library
add_library(tMath INTERFACE)
target_include_directories(tMath INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/tMath)

# tSimd library
add_library(tSimd INTERFACE)
target_include_directories(tSimd INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/tSimd)


if(TMATH_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


if(TMATH_BUILD_TESTS)
    set(BUILD_GMOCK OFF)
    add_subdirectory(3rdparty/googletest)

    enable_testing()
    add_subdirectory(tests)
endif()

if(TMATH_BUILD_BENCHMARKS)
    set(TMATH_JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/json/single_include)
    set(TMATH_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
    set(TMATH_WEBSITE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/website)

    set(BENCHMARK_ENABLE_TESTING OFF)
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
    add_subdirectory(3rdparty/benchmark)

    enable_testing()
    add_subdirectory(benchmark)
endif()
