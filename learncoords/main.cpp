//
// Created by Liu Yuchuan on 2020/3/1.
//

#include <stb_image.h>
#include <Common.h>
#include <AutoRelease.h>
#include <GLFWWrapper.h>
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const static auto VIEW_WIDTH = 800;
const static auto VIEW_HEIGHT = 600;

#define GROUND_TEST 0
#define CUBE_TEST 1

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
    int modelLocation;
    int worldLocation;
    int projectionLocation;

    glm::vec3 *cubePositions[10];

public:
    LearnTextureLoop() {
        cubePositions[0] = new glm::vec3(0.0f, 0.0f, 0.0f);
        cubePositions[1] = new glm::vec3(2.0f, 5.0f, -15.0f);
        cubePositions[2] = new glm::vec3(-1.5f, -2.2f, -2.5f);
        cubePositions[3] = new glm::vec3(-3.8f, -2.0f, -12.3f);
        cubePositions[4] = new glm::vec3(2.4f, -0.4f, -3.5f);
        cubePositions[5] = new glm::vec3(-1.7f, 3.0f, -7.5f);
        cubePositions[6] = new glm::vec3(1.3f, -2.0f, -2.5f);
        cubePositions[7] = new glm::vec3(1.5f, 2.0f, -2.5f);
        cubePositions[8] = new glm::vec3(1.5f, 0.2f, -1.5f);
        cubePositions[9] = new glm::vec3(-1.3f, 1.0f, -1.5f);
    };

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
        for (auto pPos: cubePositions) {
            delete (pPos);
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

#if GROUND_TEST

    void applyGroundTestTransform() {
        glm::mat4 model(1);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1, 0, 0));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glm::mat4 world(1);
        world = glm::translate(world, glm::vec3(0, 0, -3.0f));
        glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(world));
        glm::mat4 projection(1);
        projection = glm::perspective(glm::radians(45.0f), (float) VIEW_WIDTH / VIEW_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    }

#endif

#if  CUBE_TEST

    void applyCubeTransform() {
        glm::mat4 world(1);
        world = glm::translate(world, glm::vec3(0, 0, -3.0f));
        glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(world));
        glm::mat4 projection(1);
        projection = glm::perspective(glm::radians(45.0f), (float) VIEW_WIDTH / VIEW_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    }

#endif

    void onStartLoop(GLFWWrapper *wrapper) override {
#if CUBE_TEST
        // 立方体坐标
        float vertices[] = {
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
        };
#else
        float vertices[] = {
                //   - 位置 -          - 颜色 -     - 纹理坐标 -
                0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
                0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
                -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // 左上
        };
#endif

        int indexes[] = {
                0, 1, 3,
                1, 2, 3
        };

        shader = new Shader("res/vertex_shader.glsl", "res/fragment_shader.glsl");
        shader->use();
        shader->setInt("texture1", 0);
        shader->setInt("texture2", 1);
        modelLocation = shader->getUniformLocation("model");
        worldLocation = shader->getUniformLocation("world");
        projectionLocation = shader->getUniformLocation("projection");
#if GROUND_TEST
        applyGroundTestTransform();
#elif CUBE_TEST
        applyCubeTransform();
#else
        glm::mat4 matrix(1.0f);
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix));
        glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(matrix));
        float aspect = (float) VIEW_WIDTH / VIEW_HEIGHT;
        matrix = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(matrix));
#endif

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#if CUBE_TEST
        int aPositionLocation = shader->getAttributeLocation("aPos");
        glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(aPositionLocation);
        int aTexCoord = shader->getAttributeLocation("aTexCoord");
        glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(aTexCoord);
        glEnable(GL_DEPTH_TEST);
#else
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
        int aPositionLocation = shader->getAttributeLocation("aPos");
        glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
        glEnableVertexAttribArray(aPositionLocation);
        int aTexCoord = shader->getAttributeLocation("aTexCoord");
        glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(aTexCoord);
#endif
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void onLoop(GLFWWrapper *wrapper) override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
#if CUBE_TEST
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
        glClear(GL_COLOR_BUFFER_BIT);
#endif
        if (shader != nullptr) {
            shader->use();
            glBindVertexArray(vao);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1Obj);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2Obj);
#if CUBE_TEST
            for (int i = 0; i < 10; i++) {
                glm::mat4 model(1.0f);
                model = glm::translate(model, *cubePositions[i]);
                float angle = glm::radians(20.0f * (float) i) + ((i == 0 || i % 3 == 0) ?
                                                                 0.0f : (float) glfwGetTime() * glm::radians(50.0f));
                model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
#else
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
#endif
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
    GLFWWrapper glfwWrapper(VIEW_WIDTH, VIEW_HEIGHT, "LearnTexture");
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