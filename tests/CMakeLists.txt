function(action_for_test_target TARGET_NAME)
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /arch:AVX2 /arch:AVX) # /arch:SSE4.2 /arch:SSE2 现在对MSVC无效
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -mavx2 -mfma -mf16c -msse4.1 -msse3 -msse2)
    endif()

    target_compile_definitions(${TARGET_NAME} PRIVATE TMATH_IS_TESTING)
    target_link_libraries(${TARGET_NAME} PRIVATE gtest gtest_main tMath)

    get_target_property(OPTIONS ${TARGET_NAME} COMPILE_OPTIONS)
    message(STATUS "${TARGET_NAME} compile options: ${OPTIONS}")
endfunction()

if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(UNICODE _UNICODE)
endif()

# algorithm test
add_executable(test_algorithm test_algorithm.cpp)
action_for_test_target(test_algorithm)

# float
add_executable(test_float test_float.cpp)
action_for_test_target(test_float)

# Vector2
add_executable(test_vector2 test_vector2.cpp)
action_for_test_target(test_vector2)

# Vector3
add_executable(test_vector3 test_vector3.cpp)
action_for_test_target(test_vector3)

# Vector4
add_executable(test_vector4 test_vector4.cpp)
action_for_test_target(test_vector4)

# quat
add_executable(test_quat test_quat.cpp)
action_for_test_target(test_quat)

# operators
add_executable(test_operators_override test_operator_override.cpp)
action_for_test_target(test_operators_override)



# SIMD float4
# SSE2
add_executable(test_simd_SSE2 simd/SSE2.cpp)
action_for_test_target(test_simd_SSE2)

# SSE3
add_executable(test_simd_SSE3 simd/SSE3.cpp)
action_for_test_target(test_simd_SSE3)

# SSE4.1
add_executable(test_simd_SSE4_1 simd/SSE4_1.cpp)
action_for_test_target(test_simd_SSE4_1)

# AVX
add_executable(test_simd_AVX simd/AVX.cpp)
action_for_test_target(test_simd_AVX)

# FMA3
add_executable(test_simd_FMA3 simd/FMA3.cpp)
action_for_test_target(test_simd_FMA3)

# F16C
add_executable(test_simd_F16C simd/F16C.cpp)
action_for_test_target(test_simd_F16C)

# AVX2
add_executable(test_simd_AVX2 simd/AVX2.cpp)
action_for_test_target(test_simd_AVX2)

# no SIMD
add_executable(test_no_SIMD simd/no_simd.cpp)
action_for_test_target(test_no_SIMD)

# no SVML
add_executable(test_no_SVML simd/no_SVML.cpp)
action_for_test_target(test_no_SVML)

# compile time vector1
add_executable(test_compile_time_vector1 compile_time/test_vector1.cpp)
action_for_test_target(test_compile_time_vector1)


# 添加测试
add_test(NAME Test_Algorithm COMMAND $<TARGET_FILE:test_algorithm>)
add_test(NAME Test_Float COMMAND $<TARGET_FILE:test_float>)
add_test(NAME Test_Vector2 COMMAND $<TARGET_FILE:test_vector2>)
add_test(NAME Test_Vector3 COMMAND $<TARGET_FILE:test_vector3>)
add_test(NAME Test_Vector4 COMMAND $<TARGET_FILE:test_vector4>)
add_test(NAME Test_Quat COMMAND $<TARGET_FILE:test_quat>)
add_test(NAME Test_OperatorOverride COMMAND $<TARGET_FILE:test_operators_override>)

# SIMDs
add_test(NAME Test_simd_SSE2 COMMAND $<TARGET_FILE:test_simd_SSE2>)
add_test(NAME Test_simd_SSE3 COMMAND $<TARGET_FILE:test_simd_SSE3>)
add_test(NAME Test_simd_SSE4_1 COMMAND $<TARGET_FILE:test_simd_SSE4_1>)
add_test(NAME Test_simd_AVX COMMAND $<TARGET_FILE:test_simd_AVX>)
add_test(NAME Test_simd_FMA3 COMMAND $<TARGET_FILE:test_simd_FMA3>)
add_test(NAME Test_simd_F16C COMMAND $<TARGET_FILE:test_simd_F16C>)
add_test(NAME Test_simd_AVX2 COMMAND $<TARGET_FILE:test_simd_AVX2>)
add_test(NAME Test_no_SIMD COMMAND $<TARGET_FILE:test_no_SIMD>)
add_test(NAME Test_no_SVML COMMAND $<TARGET_FILE:test_no_SVML>)

# compile time
add_test(NAME Test_compile_time_Vector1 COMMAND $<TARGET_FILE:test_compile_time_vector1>)
