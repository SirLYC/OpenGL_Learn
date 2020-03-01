#include <glad/glad.h>
#include <Common.h>
#include "ShaderUtils.h"
#include <GLFWWrapper.h>

#define ANOTHER_PROGRAM 0

class HelloTriangle : public GLFWDefaultEventLoop {

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    int programId = 0;
    bool isInit = false;

#if ANOTHER_PROGRAM
    int anotherProgramId = 0;
#endif

public:

    ~HelloTriangle() {
        if (VAO != 0) {
            glDeleteVertexArrays(1, &VAO);
            VAO = 0;
        }
        if (VBO != 0) {
            glDeleteBuffers(1, &VBO);
            VBO = 0;
        }
        if (EBO != 0) {
            glDeleteBuffers(1, &EBO);
            EBO = 0;
        }
        if (programId != 0) {
            glDeleteProgram(programId);
        }
#if ANOTHER_PROGRAM
        if (anotherProgramId != 0) {
            glDeleteProgram(anotherProgramId);
        }
#endif
    }

    bool init() {
        if (isInit) {
            return true;
        }
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        return (isInit = VAO > 0 && VBO > 0 && EBO > 0);
    }

    void onStartLoop(GLFWWrapper *wrapper) override;

    void onLoop(GLFWWrapper *wrapper) override;

    void onStopLoop(GLFWWrapper *wrapper) override;
};


int main() {

    GLFWWrapper glfwWrapper(800, 600, "HelloTriangle");

    if (!glfwWrapper.init()) {
        return -1;
    }
    glfwWrapper.addDefaultWindowConfig();

    HelloTriangle helloTriangle;
    if (!helloTriangle.init()) {
        return -1;
    }
    glfwWrapper.runDefaultEventLoop(helloTriangle);

    return 0;
}

void HelloTriangle::onStartLoop(GLFWWrapper *wrapper) {
    char simpleVertexShader[512];
    char simpleFragShader[512];
    readFileAsText("res/vertex_shader.glsl", simpleVertexShader, 512);
    readFileAsText("res/fragment_shader.glsl", simpleFragShader, 512);
    programId = createGLProgram(simpleVertexShader, simpleFragShader);

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

#if ANOTHER_PROGRAM
    readFileAsText("res/vertex_shader.glsl", simpleVertexShader, 512);
    readFileAsText("res/fragment_shader_yellow.glsl", simpleFragShader, 512);
    anotherProgramId = createGLProgram(simpleVertexShader, simpleFragShader);
#endif
}

void HelloTriangle::onLoop(GLFWWrapper *wrapper) {
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

void HelloTriangle::onStopLoop(GLFWWrapper *wrapper) {

}
