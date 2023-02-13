#version 330 core

out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube u_Cube;

void main() {
    FragColor = texture(u_Cube, TexCoord);
}