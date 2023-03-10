layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 CurrPos;

uniform mat4 u_Model;
uniform mat4 u_Camera;

void main() {
    CurrPos = vec3(u_Model * aPos);
    gl_Position = u_Camera * vec4(CurrPos, 1.0f);
    
    TexCoord = aTexCoord;
    Normal = aNormal;
}