#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Common.h>
#include "ShaderUtils.h"

#define ANOTHER_PROGRAM 1

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

#if ANOTHER_PROGRAM

void drawFrame(int, int, unsigned int);

#else
void drawFrame(int, unsigned int);
#endif

int initProgram(unsigned int &, unsigned int &, unsigned int &);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearOpenGL", nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    int programId = initProgram(VAO, VBO, EBO);

#if ANOTHER_PROGRAM
    char simpleVertexShader[512];
    char simpleFragShader[512];
    readFileAsText("shaders/vertex_shader.glsl", simpleVertexShader, 512);
    readFileAsText("shaders/fragment_shader_yellow.glsl", simpleFragShader, 512);
    int anotherProgramId = createGLProgram(simpleVertexShader, simpleFragShader);
#endif

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
#if ANOTHER_PROGRAM
        drawFrame(programId, anotherProgramId, VAO);
#else
        drawFrame(programId, VAO);
#endif
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

#if ANOTHER_PROGRAM

void drawFrame(int programId, int anotherProgramId, unsigned int VAO)
#else
void drawFrame(int programId, unsigned int VAO)
#endif

{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glUseProgram(programId);
#if ANOTHER_PROGRAM
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(anotherProgramId);
    glDrawArrays(GL_TRIANGLES, 1, 3);
#else
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
#endif

    glBindVertexArray(0);
}

int initProgram(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO) {
    char simpleVertexShader[512];
    char simpleFragShader[512];
    readFileAsText("shaders/vertex_shader.glsl", simpleVertexShader, 512);
    readFileAsText("shaders/fragment_shader.glsl", simpleFragShader, 512);
    int programId = createGLProgram(simpleVertexShader, simpleFragShader);
    std::cout << "ProgramId=" << programId << std::endl;

    float vertices[] = {
            -0.5f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };

    unsigned int indices[] = {
            0, 1, 2,
            1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    int aPositionPosition = glGetAttribLocation(programId, "aPosition");
    glVertexAttribPointer(aPositionPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(aPositionPosition);
    glBindVertexArray(0);
    // 这个buffer一定要在bindVertexArray后取消绑定
    // 否则会修改VAO的状态
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return programId;
}
