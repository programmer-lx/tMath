# float
add_executable(test_float test_float.cpp)
target_link_libraries(test_float PRIVATE tMath)

# VectorN
add_executable(test_vector test_vector.cpp)
target_link_libraries(test_vector PRIVATE tMath)

# quat
add_executable(test_quat test_quat.cpp)
target_link_libraries(test_quat PRIVATE tMath)
