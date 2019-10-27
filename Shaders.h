//
// Created by 97269 on 2019/10/27.
//

#ifndef OPENGL_LEARN_SHADERS_H
#define OPENGL_LEARN_SHADERS_H

#define SIMPLE_VERTEX_SHADER "layout (location = 0) in vec3 aPosition;\n" \
"\n" \
"void main() {\n" \
"    gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);\n" \
"}"

#define SIMPLE_FRAG_SHADER "out vec4 FragColor;\n" \
"\n" \
"void main() {\n" \
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" \
"}"

#endif //OPENGL_LEARN_SHADERS_H
