//
// Created by 97269 on 2019/10/27.
//

#ifndef OPENGL_LEARN_SHADERUTILS_H
#define OPENGL_LEARN_SHADERUTILS_H

#include <glad/glad.h>
#include <iostream>

inline int compileSourceShader(int type, const char *source) {
    int shaderId = glCreateShader(type);
    if (shaderId == 0) {
        return 0;
    }

    glShaderSource(shaderId, 1, &source, nullptr);
    glCompileShader(shaderId);

    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        std::cerr << "Compile Shader failed! Info=" << infoLog << std::endl;
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

inline int createGLProgram(const char *vertexShaderSource, const char *fragShaderSource) {
    int vertexShaderId = compileSourceShader(GL_VERTEX_SHADER, vertexShaderSource);
    std::cout << "vertexShaderId=" << vertexShaderId << std::endl;
    if (vertexShaderId == 0) {
        return 0;
    }
    int fragShaderId = compileSourceShader(GL_FRAGMENT_SHADER, fragShaderSource);
    std::cout << "fragShaderId=" << fragShaderId << std::endl;
    if (fragShaderId == 0) {
        return 0;
    }
    int programId = glCreateProgram();

    if (programId == 0) {
        return 0;
    }

    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragShaderId);
    glLinkProgram(programId);
    int success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        std::cerr << "Cannot link program! InfoLog=" << infoLog << std::endl;
        glDeleteProgram(programId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragShaderId);
        return 0;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragShaderId);
    return programId;
}

#endif //OPENGL_LEARN_SHADERUTILS_H
