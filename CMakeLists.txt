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

if(TMATH_BUILD_TESTS)

    add_compile_definitions(TMATH_IS_TESTING)

    if(MSVC)
        add_compile_options(/arch:AVX2)
    else()
        add_compile_options(-mavx2 -mfma -mf16c -msse4.1 -msse3 -msse2)
    endif()

    enable_testing()
    add_subdirectory(tests)
endif()
