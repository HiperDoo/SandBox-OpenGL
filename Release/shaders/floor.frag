out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 CurrPos;

uniform sampler2D u_Texture_0;
uniform sampler2D u_Texture_1;
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

#define AMBIENT 0.2f
#define SPECULAR_LIGHT 0.5f;

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
			texture(u_Texture_0, TexCoord) *
			(diffuse * inten + AMBIENT) +
			texture(u_Texture_1, TexCoord).r * specular * inten
		) * u_LightColor;
}

vec4 directLight() {
	// Diffuse Lighting
	vec3 normalized = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normalized, lightDirection), 0.0f);

	// Specular Lighting
	vec3 viewDirection = normalize(u_CameraPos - CurrPos);
	vec3 reflectionDirection = reflect(-lightDirection, normalized);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * SPECULAR_LIGHT;

	return (
			texture(u_Texture_0, TexCoord) *
			(diffuse + AMBIENT) +
			texture(u_Texture_1, TexCoord).r * specular
		) * u_LightColor;
}

// Mismo resultado que pointLight(), sepa porque fregados, pero esta mal
vec4 spotLight() {
	const float outerCone = 0.9f, innerCone = 0.95f;

	// Diffuse Lighting
	vec3 normalized = normalize(Normal);
	vec3 lightDirection = normalize(u_LightPos - CurrPos);
	float diffuse = max(dot(normalized, lightDirection), 0.0f);

	// Specular Lighting
	vec3 viewDirection = normalize(u_CameraPos - CurrPos);
	vec3 reflectionDirection = reflect(-lightDirection, normalized);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * SPECULAR_LIGHT;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle / outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (
			texture(u_Texture_0, TexCoord) *
			(diffuse * inten + AMBIENT) +
			texture(u_Texture_1, TexCoord).r * specular * inten
		) * u_LightColor;
}

void main() {
    FragColor = pointLight();
	FragColor.a = 1.0f;
}