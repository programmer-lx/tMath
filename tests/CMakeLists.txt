set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_LINK_WARNING_AS_ERROR ON)

if(MSVC)
    add_compile_options(/WX)
    add_link_options(/WX)
else()
    add_compile_options(-Wall -Werror)
#    add_link_options(-Wl --fatal-warnings)
endif()

add_subdirectory(tMath)
add_subdirectory(tSimd)