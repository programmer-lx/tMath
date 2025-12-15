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

if(MSVC)
    target_compile_definitions(tMath INTERFACE UNICODE _UNICODE)
    target_compile_options(tMath INTERFACE /utf-8)
endif()


# options of test
option(TMATH_BUILD_TESTS "Build tests?" OFF)
option(TMATH_TEST_ENABLE_AVX2 "enable avx2 (test only)" OFF)

if(TMATH_BUILD_TESTS)
    if(TMATH_TEST_ENABLE_AVX2)
        if(MSVC)
            add_compile_options(/arch:AVX2)
        else()
            add_compile_options(-mavx2)
        endif()
    endif()

    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
