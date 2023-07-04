out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 ToCameraVec;
in vec3 ToLightVec;

uniform sampler2D u_Diffuse_0;
uniform vec3 u_LightColor;

#define AMBIENT_LIGHT 0.2f
#define u_ShineDamper 10
#define u_Reflectivity 1

void main() {
	// Igualar el vector a un rango entre 0 y 1
	vec3 unitNormal = normalize(Normal);
	// Vector de la luz al modelo
	vec3 unitLightVector = normalize(ToLightVec);

	// Si ambos apuntan a la misma direccion es 1, y si son
	// perpendiculares (90 grad) es 0, tambien hay decimales.
	// Determina que tan brillante es el pixel.
	float nDotl = dot(unitNormal, unitLightVector);
	float brightness = max(nDotl, AMBIENT_LIGHT); // Que la luz no sea menor a AMBIENT_LIGHT
	vec3 diffuse = brightness * u_LightColor;

	// Vector del modelo a la camara
	vec3 unitCameraVector = normalize(ToCameraVec);
	vec3 lightDirection = -unitLightVector;
	// Vector de la luz reflejada
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflectedLightDirection, unitCameraVector);
	specularFactor = max(specularFactor, 0.0f);
	float dampedFactor = pow(specularFactor, u_ShineDamper);
	vec3 finalspecular = dampedFactor * u_LightColor * u_Reflectivity;


	FragColor = vec4(diffuse, 1.0f) * texture(u_Diffuse_0, TexCoord) + vec4(finalspecular, 1.0f);
}