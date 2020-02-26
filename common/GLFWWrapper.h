//
// Created by Liu Yuchuan on 2020/2/26.
//

#ifndef OPENGL_LEARN_GLFWWRAPPER_H
#define OPENGL_LEARN_GLFWWRAPPER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <Common.h>

class GLFWWrapper;

class GLFWDefaultEventLoop {
public:
    virtual void onStartLoop(GLFWWrapper *wrapper) = 0;

    virtual void onLoop(GLFWWrapper *wrapper) = 0;

    virtual void onStopLoop(GLFWWrapper *wrapper) = 0;

    bool redrawWaitingForEvent() {
        return false;
    };
};

class GLFWWrapper {
public:
    GLFWWrapper(int width, int height, const char *title);

    ~GLFWWrapper();

    template<int majorVersion = 3, int minorVersion = 3>
    bool init();

    void addDefaultWindowConfig();

    void runDefaultEventLoop(GLFWDefaultEventLoop &defaultEventLoop);

    void defaultProcessInput();

private:
    GLFWwindow *pWindow = nullptr;
    const int width;
    const int height;
    std::string title;
    bool isInit = false;
};

GLFWWrapper::GLFWWrapper(int width, int height, const char *title) : width(width), height(height),
                                                                     title(std::string(title)) {

}

GLFWWrapper::~GLFWWrapper() {
    glfwTerminate();
    pWindow = nullptr;
}

void GLFWWrapper::addDefaultWindowConfig() {
    if (pWindow == nullptr) {
        return;
    }
    glfwSetWindowSizeCallback(pWindow, [](GLFWwindow *window, int w, int h) {
        glViewport(0, 0, w, h);
    });
}

void GLFWWrapper::runDefaultEventLoop(GLFWDefaultEventLoop &defaultEventLoop) {
    if (pWindow == nullptr) {
        return;
    }

    defaultEventLoop.onStartLoop(this);

    while (!glfwWindowShouldClose(pWindow)) {
        defaultProcessInput();
        defaultEventLoop.onLoop(this);
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
    defaultEventLoop.onStopLoop(this);
}

template<int majorVersion, int minorVersion>
bool GLFWWrapper::init() {
    if (isInit) {
        return true;
    }
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (pWindow == nullptr) {
        eprintf("Failed to create window");
        return false;
    }

    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        eprintf("Failed to initialize GLAD");
    }

    isInit = true;
    return true;
}

void GLFWWrapper::defaultProcessInput() {
    if (pWindow == nullptr) {
        return;
    }
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, true);
    }
}

#endif //OPENGL_LEARN_GLFWWRAPPER_H
