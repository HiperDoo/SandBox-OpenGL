#version 330 core

out vec4 FragColor;

uniform vec4 u_LightColor;

void main() {
	FragColor = u_LightColor;
}