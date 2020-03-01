//
// Created by Liu Yuchuan on 2020/3/1.
//

#include "Shader.h"
#include "ShaderUtils.h"

void Shader::use() {
    glUseProgram(programId);
}

void Shader::setBool(const char *name, bool value) const {
    glUniform1i(glGetUniformLocation(programId, name), value ? 1 : 0);
}

void Shader::setInt(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(programId, name), value);
}

void Shader::setFloat(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(programId, name), value);
}

Shader::~Shader() {
    if (programId != 0) {
        glDeleteProgram(programId);
    }
}

int Shader::getAttributeLocation(const char *name) const {
    return glGetAttribLocation(programId, name);
}
