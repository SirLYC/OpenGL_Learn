//
// Created by Liu Yuchuan on 2020/3/1.
//

#include <stb_image.h>
#include <Common.h>
#include <AutoRelease.h>
#include <GLFWWrapper.h>
#include <Shader.h>

class LearnTextureLoop : public GLFWDefaultEventLoop {
private:
    int width1;
    int height1;
    int channel1;
    GLuint texture1Obj = 0;
    int width2;
    int height2;
    int channel2;
    GLuint texture2Obj = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    Shader *shader = nullptr;

public:
    ~LearnTextureLoop() {
        if (texture1Obj != 0) {
            glDeleteTextures(1, &texture1Obj);
        }
        if (texture2Obj != 0) {
            glDeleteTextures(1, &texture2Obj);
        }
        if (vao != 0) {
            glDeleteVertexArrays(1, &vao);
        }
        if (vbo != 0) {
            glDeleteBuffers(1, &vbo);
        }
        if (ebo != 0) {
            glDeleteBuffers(1, &ebo);
        }
        if (shader != nullptr) {
            delete shader;
            shader = nullptr;
        }
    }

    bool init() {
        unsigned char *data = nullptr;
        bool success = true;
        autoRelease([&data, &success, this] {
            data = stbi_load("res/container.jpg", &width1, &height1, &channel1, 0);
            if (data == nullptr) {
                success = false;
                return;
            }

            glGenTextures(1, &texture1Obj);
            glBindTexture(GL_TEXTURE_2D, texture1Obj);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

        }, [&data] {
            if (data != nullptr) {
                stbi_image_free(data);
                printf("Freed image data!\n");
            }
        });

        printf("Image w=%d, height=%d, channel=%d\n", width1, height1, channel1);
        if (!success) {
            eprintf("Cannot open image 1 file!\n");
            return false;
        }

        autoRelease([&data, &success, this] {
            data = stbi_load("res/awesomeface.png", &width2, &height2, &channel2, 0);
            if (data == nullptr) {
                success = false;
                return;
            }

            glGenTextures(1, &texture2Obj);
            glBindTexture(GL_TEXTURE_2D, texture2Obj);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }, [&data] {
            if (data != nullptr) {
                stbi_image_free(data);
                printf("Freed image data!\n");
            }
        });
        if (!success) {
            eprintf("Cannot open image2 file!\n");
            return false;
        }
        printf("Image w=%d, height=%d, channel=%d\n", width2, height2, channel2);
        return true;
    }

    void onStartLoop(GLFWWrapper *wrapper) override {
        float vertices[] = {
                //   - 位置 -          - 颜色 -     - 纹理坐标 -
                1, 1, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
                1, -1, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
                -1, -1, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
                -1, 1, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // 左上
        };

        int indexes[] = {
                0, 1, 3,
                1, 2, 3
        };

        shader = new Shader("res/vertex_shader.glsl", "res/fragment_shader.glsl");
        shader->use();
        shader->setInt("texture1", 0);
        shader->setInt("texture2", 1);

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
        int aPositionLocation = shader->getAttributeLocation("aPos");
        glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(aPositionLocation);
        int aColorLocation = shader->getAttributeLocation("aColor");
        glVertexAttribPointer(aColorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(aColorLocation);
        int aTexCoord = shader->getAttributeLocation("aTexCoord");
        glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(aTexCoord);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void onLoop(GLFWWrapper *wrapper) override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if (shader != nullptr) {
            shader->use();
            glBindVertexArray(vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1Obj);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2Obj);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }
    }

    void onStopLoop(GLFWWrapper *wrapper) override {

    }
};

int main() {
    GLFWWrapper glfwWrapper(512, 512, "LearnTexture");
    if (!glfwWrapper.init()) {
        return -1;
    }

    glfwWrapper.addDefaultWindowConfig();
    LearnTextureLoop loop;
    if (!loop.init()) {
        return -1;
    }

    glfwWrapper.runDefaultEventLoop(loop);
}