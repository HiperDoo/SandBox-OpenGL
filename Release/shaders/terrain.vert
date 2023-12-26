layout(location = 0) in vec3 aPos;

uniform mat4 u_Camera;
out vec4 Color;

void main() {
    gl_Position = u_Camera * vec4(aPos, 1.0);

    Color = vec4(aPos.y / 200.0);
}