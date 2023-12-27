layout(location = 0) in vec3 aPos;

uniform mat4 u_Camera;
uniform float u_MinHeight;
uniform float u_MaxHeight;

out vec4 Color;

void main() {
    gl_Position = u_Camera * vec4(aPos, 1.0);

    float deltaHeight = u_MaxHeight - u_MinHeight;
    float heightRatio = (aPos.y - u_MinHeight) / deltaHeight;
    float c = (heightRatio * 0.8) + 0.2;
    Color = vec4(c, c, c, 1.0);
}