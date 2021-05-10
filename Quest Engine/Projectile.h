#pragma once

#include "Entity.h"

class Projectile : public Entity {
protected:
	Shader* _shader;

	// Initialize shader after GL
	Shader* shader() {
		if (!_shader) {
			_shader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
		}

		return _shader;
	}

public:
	static const int MAX_RANGE = 10; // TODO: tweak
	float distanceTravelled;
	glm::vec3 direction;
	float mvmtSpeed = 10.0f;
	float maxDistance = 100.0f;

	Projectile(glm::vec3 position, glm::vec3 direction, Shader* shader) : Entity(shader, "BALL1.obj") {
		this->position = position;
		this->distanceTravelled = 0.0;
		this->direction = direction;
	}

	void updatePos(float deltaTime)
	{
		this->position += this->direction * mvmtSpeed * deltaTime;
		this->distanceTravelled += mvmtSpeed * deltaTime;
	}

	bool terminate()
	{
		if (this->distanceTravelled >= maxDistance)
			return true;
		return false;
	}
};
