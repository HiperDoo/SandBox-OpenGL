out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 LightVector;
in vec3 CurrPos;

uniform sampler2D u_Diffuse_0;
uniform vec3 u_LightColor;
#define u_ShineDamper 10
//uniform float u_Reflectivity;
uniform vec3 u_CameraPos;

void main() {
    vec3 unitNormal = normalize(Normal);
    vec3 unitVectorLight = normalize(LightVector);

    float nDot = dot(unitNormal, unitVectorLight);
    float brightness = max(nDot, 0.1f);
    vec3 diffuse = brightness * u_LightColor;

    vec3 unitCamera = normalize(u_CameraPos - CurrPos);
    vec3 lightDirection = -unitCamera;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
    float specularFactor = dot(reflectedLightDirection, unitCamera);
    specularFactor = max(specularFactor, 0.0f);
    float dampedFactor = pow(specularFactor, u_ShineDamper);
    vec3 finalSpecular = dampedFactor * u_LightColor;

	FragColor = vec4(diffuse, 1.0f) * texture(u_Diffuse_0, TexCoord) + vec4(finalSpecular, 1.0f);
}