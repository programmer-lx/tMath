function(action_for_test_target TARGET_NAME)
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:/O2>)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:-O2>)
    endif()
    target_link_libraries(${TARGET_NAME} PRIVATE gtest)
endfunction()

if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(UNICODE _UNICODE)
endif()

# float
add_executable(test_float test_float.cpp)
target_link_libraries(test_float PRIVATE tMath)
action_for_test_target(test_float)

# Vector2
add_executable(test_vector2 test_vector2.cpp)
target_link_libraries(test_vector2 PRIVATE tMath)
action_for_test_target(test_vector2)

# Vector3
add_executable(test_vector3 test_vector3.cpp)
target_link_libraries(test_vector3 PRIVATE tMath)
action_for_test_target(test_vector3)

# Vector4
add_executable(test_vector4 test_vector4.cpp)
target_link_libraries(test_vector4 PRIVATE tMath)
action_for_test_target(test_vector4)

# quat
add_executable(test_quat test_quat.cpp)
target_link_libraries(test_quat PRIVATE tMath)
action_for_test_target(test_quat)

# operators
add_executable(test_operators_override test_operator_override.cpp)
target_link_libraries(test_operators_override PRIVATE tMath)
action_for_test_target(test_operators_override)



# SIMD float4
# SSE2
add_executable(test_simd_SSE2 simd/SSE2.cpp)
target_link_libraries(test_simd_SSE2 PRIVATE tMath)
action_for_test_target(test_simd_SSE2)

# SSE3
add_executable(test_simd_SSE3 simd/SSE3.cpp)
target_link_libraries(test_simd_SSE3 PRIVATE tMath)
action_for_test_target(test_simd_SSE3)

# SSE4.1
add_executable(test_simd_SSE4_1 simd/SSE4_1.cpp)
target_link_libraries(test_simd_SSE4_1 PRIVATE tMath)
action_for_test_target(test_simd_SSE4_1)

# AVX
add_executable(test_simd_AVX simd/AVX.cpp)
target_link_libraries(test_simd_AVX PRIVATE tMath)
action_for_test_target(test_simd_AVX)

# FMA3
add_executable(test_simd_FMA3 simd/FMA3.cpp)
target_link_libraries(test_simd_FMA3 PRIVATE tMath)
action_for_test_target(test_simd_FMA3)

# F16C
add_executable(test_simd_F16C simd/F16C.cpp)
target_link_libraries(test_simd_F16C PRIVATE tMath)
action_for_test_target(test_simd_F16C)

# AVX2
add_executable(test_simd_AVX2 simd/AVX2.cpp)
target_link_libraries(test_simd_AVX2 PRIVATE tMath)
action_for_test_target(test_simd_AVX2)

# no SIMD
add_executable(test_no_SIMD simd/no_simd.cpp)
target_link_libraries(test_no_SIMD PRIVATE tMath)
action_for_test_target(test_no_SIMD)


# 添加测试
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
