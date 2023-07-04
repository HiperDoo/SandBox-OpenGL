out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 CurrPos;

uniform sampler2D u_Diffuse_0;
#ifndef NO_SPECULAR
uniform sampler2D u_Specular_0;
#endif
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

#define AMBIENT 0.2f
#define SPECULAR_LIGHT 0.5f

vec4 pointLight() {
	vec3 lightVec = u_LightPos - CurrPos;
	float dist = length(lightVec);

	const float a = 0.05f, b = 0.01f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// Diffuse Lighting
	vec3 normalized = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normalized, lightDirection), 0.0f);

	// Specular Lighting
	vec3 viewDirection = normalize(u_CameraPos - CurrPos);
	vec3 reflectionDirection = reflect(-lightDirection, normalized);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * SPECULAR_LIGHT;

	return (
		texture(u_Diffuse_0, TexCoord) *
		(diffuse * inten + AMBIENT) + specular * inten
	) * vec4(u_LightColor, 1.0f);
}

void main() {
	FragColor = pointLight();
}