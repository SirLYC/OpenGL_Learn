//
// Created by Liu Yuchuan on 2020/3/1.
//

#ifndef OPENGL_LEARN_SHADER_H
#define OPENGL_LEARN_SHADER_H

#include "Common.h"

class Shader {
private:
    unsigned int programId = 0;
public:
    template<int vBufferSize = 512, int fBufferSize = 512>
    Shader(const char *vertexPath, const char *fragmentPath);

    ~Shader();

    void use();

    void setBool(const char *name, bool value) const;

    void setInt(const char *name, int value) const;

    void setFloat(const char *name, float value) const;

    int getAttributeLocation(const char *name) const;
};

template<int vBufferSize, int fBufferSize>
Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    char vertexShader[vBufferSize];
    char fragShader[fBufferSize];
    readFileAsText(vertexPath, vertexShader, vBufferSize);
    readFileAsText(fragmentPath, fragShader, fBufferSize);
    programId = createGLProgram(vertexShader, fragShader);
}


#endif //OPENGL_LEARN_SHADER_H
