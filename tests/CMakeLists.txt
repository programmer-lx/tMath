function(add_o3_optimization TARGET_NAME)
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:/Ox /GL>)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE $<$<CONFIG:Release>:-O3>)
    endif()
endfunction()

if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(UNICODE _UNICODE)
endif()

# float
add_executable(test_float test_float.cpp)
target_link_libraries(test_float PRIVATE tMath)
add_o3_optimization(test_float)

# VectorN
add_executable(test_vector test_vector.cpp)
target_link_libraries(test_vector PRIVATE tMath)
add_o3_optimization(test_vector)

# quat
add_executable(test_quat test_quat.cpp)
target_link_libraries(test_quat PRIVATE tMath)
add_o3_optimization(test_quat)

# operators
add_executable(test_operators_override test_operator_override.cpp)
target_link_libraries(test_operators_override PRIVATE tMath)
add_o3_optimization(test_operators_override)

# SIMD vec4 float32
add_executable(test_simd_vector4_float32 simd/test_simd_vector4_float32.cpp)
target_link_libraries(test_simd_vector4_float32 PRIVATE tMath)
add_o3_optimization(test_simd_vector4_float32)


# 添加测试
add_test(NAME TestFloat COMMAND $<TARGET_FILE:test_float>)
add_test(NAME TestVector COMMAND $<TARGET_FILE:test_vector>)
add_test(NAME TestQuat COMMAND $<TARGET_FILE:test_quat>)
add_test(NAME TestOperatorOverride COMMAND $<TARGET_FILE:test_operators_override>)
add_test(NAME TestSimdVector4_float32 COMMAND $<TARGET_FILE:test_simd_vector4_float32>)
