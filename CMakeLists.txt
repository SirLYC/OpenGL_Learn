cmake_minimum_required(VERSION 3.10)
project(OpenGL_Learn)

set(CMAKE_CXX_STANDARD 11)
include_directories(includes)
link_directories(libs/GLFW)

add_executable(OpenGL_Learn main.cpp libs/glad/glad.c)
target_link_libraries(OpenGL_Learn glfw3)
