#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

// texture sampler
uniform sampler2D texture2;

void main()
{
    //    FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0f);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1-TexCoord.s, 1 - TexCoord.t)), 0.2f);
}