#version 330 core

in vec3 fragPos;
in vec3 normal;

out FragColor;

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
uniform Light pointLights[numLights];

uniform float shininess;

vec3 pointLightCalc(Light light, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float Ka = 0.1;

	float Kd = max(dot(normal, lightDir), 0.0);

	vec3 reflection = reflect(-lightDir, normal);
	float Ks = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 ambient = light.ambient * Ka * attenuation * lightColor;
	vec3 diffuse = light.diffuse * Kd * attenuation * light.color;
	vec3 specular = light.specular * Ks * attenuation * light.color;

	return ambient + diffuse + specular;
}

void main()
{
	vec3 fragColor = vec3(0.0, 0.0, 0.0);
	normal = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	int i;
	for (i = 0; i < numLights; i++)
		FragColor += pointLightCalc(pointLights[i], viewDir);

	fragColor *= vec3(0.0, 0.5, 0.9);

	return vec4(fragColor, 1.0);
}