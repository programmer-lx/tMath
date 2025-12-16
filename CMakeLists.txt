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
option(TMATH_BUILD_TESTS "Build tests?"                 OFF)
option(TMATH_TEST_SSE2   "Enable SSE2 for testing"      OFF)
option(TMATH_TEST_SSE3   "Enable SSE3 for testing"      OFF)
option(TMATH_TEST_SSE4_1 "Enable SSE4.1 for testing"    OFF)
option(TMATH_TEST_AVX    "Enable AVX for testing"       OFF)
option(TMATH_TEST_AVX2   "Enable AVX2 for testing"      OFF)
option(TMATH_TEST_FMA3   "Enable FMA3 for testing"      OFF)
option(TMATH_TEST_F16C   "Enable F16C for testing"      OFF)
#option(TMATH_TEST_NEON   "Enable NEON for testing"      OFF)


if(TMATH_BUILD_TESTS)

    add_compile_definitions(TMATH_IS_TESTING)
    add_compile_definitions(
            $<$<BOOL:${TMATH_TEST_SSE2}>:TMATH_TEST_SSE2>
            $<$<BOOL:${TMATH_TEST_SSE3}>:TMATH_TEST_SSE3>
            $<$<BOOL:${TMATH_TEST_SSE4_1}>:TMATH_TEST_SSE4_1>
            $<$<BOOL:${TMATH_TEST_AVX}>:TMATH_TEST_AVX>
            $<$<BOOL:${TMATH_TEST_AVX2}>:TMATH_TEST_AVX2>
            $<$<BOOL:${TMATH_TEST_FMA3}>:TMATH_TEST_FMA3>
            $<$<BOOL:${TMATH_TEST_F16C}>:TMATH_TEST_F16C>
#            $<$<BOOL:${TMATH_TEST_NEON}>:TMATH_TEST_ARM_NEON>
    )

    if(MSVC)
        add_compile_options(/arch:AVX2)
    else()
        add_compile_options(-mavx2 -mfma -mf16c -msse4.1 -msse3 -msse2)
    endif()

    enable_testing()
    add_subdirectory(tests)
endif()
