#version 330 core

out vec4 FragColor;

uniform vec4 LightColor;

void main() {
	FragColor = LightColor;
}