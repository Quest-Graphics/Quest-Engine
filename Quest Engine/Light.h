#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Light
{
	glm::vec3 position;
	glm::vec3 color;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

Light* light(glm::vec3 position = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f), glm::vec3 ambient = glm::vec3(0.0f), glm::vec3 diffuse = glm::vec3(0.0f), glm::vec3 specular = glm::vec3(0.0f),
		float constant = 1.0f, float linear = 0.0f, float quadratic = 0.0f)
{
	Light* light = new Light;
	light->position = position;
	light->color = color;

	light->ambient = ambient;
	light->diffuse = diffuse;
	light->specular = specular;
	
	light->constant = constant;
	light->linear = linear;
	light->quadratic = quadratic;

	return light;
}