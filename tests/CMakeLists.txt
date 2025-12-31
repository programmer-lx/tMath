function(action_for_test_target TARGET_NAME)
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /utf-8 /arch:AVX2 /arch:AVX) # /arch:SSE4.2 /arch:SSE2 现在对MSVC无效
        target_compile_definitions(${TARGET_NAME} PRIVATE UNICODE _UNICODE)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -mavx2 -mfma -mf16c -msse4.1 -msse3 -msse2)
    endif()

    target_compile_definitions(${TARGET_NAME} PRIVATE TMATH_IS_TESTING)
    target_link_libraries(${TARGET_NAME} PRIVATE gtest gtest_main tMath)

    get_target_property(OPTIONS ${TARGET_NAME} COMPILE_OPTIONS)
    message(STATUS "${TARGET_NAME} compile options: ${OPTIONS}")
endfunction()

file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

foreach(SOURCE_FILE ${TEST_SOURCES})
    # 读取文件路径，变成test target的名字: "simd/SSE2.cpp" -> "test_simd_SSE2"
    string(REPLACE "/" "_" TEST_TARGET_NAME "${SOURCE_FILE}")
    get_filename_component(TEST_TARGET_NAME ${TEST_TARGET_NAME} NAME_WE)

    add_executable(${TEST_TARGET_NAME} ${SOURCE_FILE})
    action_for_test_target(${TEST_TARGET_NAME})

    # 自动添加到 CTest
    add_test(NAME ${TEST_TARGET_NAME} COMMAND $<TARGET_FILE:${TEST_TARGET_NAME}>)

#    message(STATUS "Auto-detected test: ${TEST_TARGET_NAME} from ${SOURCE_FILE}")
endforeach()
