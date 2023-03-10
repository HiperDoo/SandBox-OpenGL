layout(location = 0) in vec4 aPos;

out vec3 TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main() {
    vec4 position = u_Projection * u_View * aPos;
    gl_Position = position.xyww;
    TexCoord = vec3(aPos.x, aPos.y, -aPos.z);
}