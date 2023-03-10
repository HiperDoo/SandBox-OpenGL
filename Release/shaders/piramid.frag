out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 CurrPos;

uniform sampler2D u_Texture;
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

#define AMBIENT 0.2f
#define SPECULAR_LIGHT 0.5f;

void main() {
	vec3 normalized = normalize(Normal);
	vec3 lightDirection = normalize(u_LightPos - CurrPos);
	float diffuse = max(dot(normalized, lightDirection), 0.0f);

	vec3 viewDirection = normalize(u_CameraPos - CurrPos);
	vec3 reflectionDirection = reflect(-lightDirection, normalized);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * SPECULAR_LIGHT;

	FragColor = texture(u_Texture, TexCoord) * u_LightColor * (diffuse + AMBIENT + specular);
	FragColor.a = 1.0f;
}