layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 LightVector;
out vec3 CurrPos;

uniform mat4 u_Model;
uniform mat4 u_Camera;
uniform vec3 u_LightPos;

void main() {
    CurrPos = vec3(u_Model * aPos);
	gl_Position = u_Camera * vec4(CurrPos, 1.0);

    Normal = aNormal;
    LightVector = u_LightPos - CurrPos;
}