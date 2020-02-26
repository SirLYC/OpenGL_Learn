cmake_minimum_required(VERSION 3.10)
project(OpenGL_Learn)

set(CMAKE_CXX_STANDARD 11)
include_directories(includes)

add_library(glad SHARED includes/glad/glad.h includes/glad/khrplatform.h libs/glad/glad.c)

file(GLOB COMMON_SRC common/Common.cpp common/Common.h)
add_library(common SHARED ${COMMON_SRC})

add_executable(OpenGL_Learn main.cpp ShaderUtils.h)

target_include_directories(OpenGL_Learn PUBLIC common)
target_link_libraries(OpenGL_Learn glfw3 glad common)
