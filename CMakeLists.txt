cmake_minimum_required(VERSION 3.10)
project(OpenGL_Learn)

set(CMAKE_CXX_STANDARD 11)
include_directories(includes common)

add_library(glad SHARED includes/glad/glad.h includes/glad/khrplatform.h libs/glad/glad.c)

file(GLOB COMMON_SRC common/Common.cpp common/Common.h)
add_library(common SHARED ${COMMON_SRC})

set(EXES HelloTriangle)

add_executable(HelloTriangle hellotriangle/main.cpp)

target_link_libraries(HelloTriangle glfw3 glad common)

foreach (exe ${EXES})
    add_custom_command(TARGET ${exe} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${exe}>/shaders)
endforeach ()
