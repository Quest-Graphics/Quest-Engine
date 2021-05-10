#version 330 core

in vec3 fragPos;
in vec3 N;
in vec2 TexCoords;

out vec4 FragColor;

struct Light
{
	vec3 position;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform vec3 viewPos;
uniform int numLights;
uniform Light pointLights[50];
uniform vec3 objColor;
uniform sampler2D ourTexture;

uniform float shininess;

vec3 pointLightCalc(Light light, vec3 viewDir, vec3 normal)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float Ka = 1.0;

	float Kd = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float Ks = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 ambient = light.ambient * Ka * light.color * texture(ourTexture, TexCoords).xyz;
	vec3 diffuse = light.diffuse * Kd * light.color * texture(ourTexture, TexCoords).xyz;
	vec3 specular = light.specular * Ks * light.color * texture(ourTexture, TexCoords).xyz;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

void main()
{
	vec3 fragColor = vec3(0.0, 0.0, 0.0);
	vec3 normal = normalize(N);
	vec3 viewDir = normalize(viewPos - fragPos);

	int i;
	for (i = 0; i < numLights; i++)
		fragColor += pointLightCalc(pointLights[i], viewDir, normal);

	FragColor = vec4(fragColor, 1.0);
}