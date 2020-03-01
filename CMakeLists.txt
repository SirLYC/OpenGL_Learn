cmake_minimum_required(VERSION 3.10)
project(OpenGL_Learn)

set(CMAKE_CXX_STANDARD 11)
include_directories(includes common includes/stbImage)

add_library(glad SHARED includes/glad/glad.h includes/glad/khrplatform.h libs/glad/glad.c)

file(GLOB COMMON_SRC common/*.cpp common/*.h)
add_library(common SHARED ${COMMON_SRC})
target_link_libraries(common glfw3 glad)

add_library(stbImage includes/stbImage/stb_image.h libs/stbImage/stb_image.c)
target_compile_definitions(stbImage PUBLIC STB_IMAGE_IMPLEMENTATION)

set(EXES HelloTriangle glsllearn-1 glsllearn-2)

foreach (exe ${EXES})
    file(GLOB SRC ${exe}/*.cpp ${exe}/*.c ${exe}/*.h ${exe}/*.hpp)
    add_executable(${exe} ${SRC})
    target_link_libraries(${exe} glfw3 glad common)
    add_custom_command(TARGET ${exe} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${exe}/res $<TARGET_FILE_DIR:${exe}>/res)
endforeach ()
