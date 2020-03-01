#include <GLFWWrapper.h>
#include <ShaderUtils.h>
#include <Common.h>
#include <cmath>

#define CONTROL_COLOR 1

class GLSLLearn2Loop : public GLFWDefaultEventLoop {
private:
    GLuint vertexArrayObj = 0;
    GLuint arrayBufferObj = 0;
    int programId = 0;

public:

    ~GLSLLearn2Loop() {
        if (vertexArrayObj != 0) {
            glDeleteVertexArrays(1, &vertexArrayObj);
            vertexArrayObj = 0;
        }
        if (arrayBufferObj != 0) {
            glDeleteBuffers(1, &arrayBufferObj);
            arrayBufferObj = 0;
        }
        if (programId != 0) {
            glDeleteProgram(programId);
        }
    }

    void onStartLoop(GLFWWrapper *wrapper) override {
        char vertexShader[512];
        char fragShader[512];
#if CONTROL_COLOR
        readFileAsText("res/vertex_shader_use_input.glsl", vertexShader, 512);
#else
        readFileAsText("res/vertex_shader.glsl", vertexShader, 512);
#endif
        readFileAsText("res/fragment_shader.glsl", fragShader, 512);
        programId = createGLProgram(vertexShader, fragShader);

        glGenVertexArrays(1, &vertexArrayObj);
        glGenBuffers(1, &arrayBufferObj);

        glBindVertexArray(vertexArrayObj);
        glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj);

#if CONTROL_COLOR
        float locations[] = {
                0, 0.5f, 0, 1, 0, 0,
                0.5f, -0.5f, 0, 0, 1, 0,
                -.5f, -.5f, 0, 0, 0, 1
        };
#else
        float locations[] = {
                0, 0.5f, 0,
                0.5f, -0.5f, 0,
                -.5f, -.5f, 0
        };
#endif
        glBufferData(GL_ARRAY_BUFFER, sizeof(locations), locations, GL_STATIC_DRAW);
        auto aPositionLocation = glGetAttribLocation(programId, "aPosition");
#if CONTROL_COLOR
        glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
#else
        glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
#endif

        glEnableVertexAttribArray(aPositionLocation);
#if CONTROL_COLOR
        auto aColorLocation = glGetAttribLocation(programId, "aColor");
        glVertexAttribPointer(aColorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(aColorLocation);
#endif
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void onLoop(GLFWWrapper *wrapper) override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programId);
        glBindVertexArray(vertexArrayObj);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

    void onStopLoop(GLFWWrapper *wrapper) override {

    }
};


int main() {
    GLFWWrapper glfwWrapper(800, 600, "TestUniform");
    if (!glfwWrapper.init()) {
        return -1;
    }
    glfwWrapper.addDefaultWindowConfig();
    GLSLLearn2Loop loop;
    glfwWrapper.runDefaultEventLoop(loop);
}