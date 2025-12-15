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


# options of test
option(TMATH_BUILD_TESTS "Build tests?" OFF)
option(TMATH_TEST_ENABLE_AVX2 "enable avx2 (test only)" OFF)


include(CheckCXXCompilerFlag)
function(tmath_try_enable_avx2 out_var)
    # 只考虑 x86 架构
    if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|i.86")
        set(${out_var} OFF PARENT_SCOPE)
        return()
    endif()

    if(MSVC)
        check_cxx_compiler_flag("/arch:AVX2" HAS_AVX2_FLAG)
    else()
        check_cxx_compiler_flag("-mavx2" HAS_AVX2_FLAG)
    endif()

    set(${out_var} ${HAS_AVX2_FLAG} PARENT_SCOPE)
endfunction()

if(TMATH_BUILD_TESTS)
    set(TMATH_USE_AVX2 OFF)

    if(TMATH_TEST_ENABLE_AVX2)
        tmath_try_enable_avx2(TMATH_USE_AVX2)
    endif()

    if(TMATH_USE_AVX2)
        message(STATUS "TMATH tests: AVX2 enabled")
        if(MSVC)
            add_compile_options(/arch:AVX2)
        else()
            add_compile_options(-mavx2)
        endif()
        add_compile_definitions(TMATH_ENABLE_AVX2)
    else()
        message(STATUS "TMATH tests: AVX2 disabled")
    endif()

    enable_testing()
    add_subdirectory(tests)
endif()
