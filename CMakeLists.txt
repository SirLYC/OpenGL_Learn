cmake_minimum_required(VERSION 3.10)
project(OpenGL_Learn)

set(CMAKE_CXX_STANDARD 11)
include_directories(includes)

add_executable(OpenGL_Learn main.cpp libs/glad/glad.c Shaders.h ShaderUtils.h)
target_link_libraries(OpenGL_Learn glfw3)
