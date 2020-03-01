layout (location = 0) in vec3 aPosition;

out vec3 inputColor;

void main() {
    gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
    inputColor = aPosition;
}