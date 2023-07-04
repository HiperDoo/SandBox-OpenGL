layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 ToCameraVec;
out vec3 ToLightVec;

uniform mat4 u_Model;
uniform mat4 u_Camera;
uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;

void main() {
    vec4 WorldPos = u_Model * aPos;
	gl_Position = u_Camera * WorldPos;

    TexCoord = aTexCoord;
    // Tambien debe de estar a la para con el u_Model ya que este puede rotar
    Normal = (u_Model * vec4(aNormal, 0.0f)).xyz;
    // Diferencia entre la posicion de la luz y del modelo
    ToLightVec = u_LightPos - WorldPos.xyz;
    ToCameraVec = u_CameraPos - WorldPos.xyz;
}