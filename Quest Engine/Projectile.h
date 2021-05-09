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

	Projectile(glm::vec3 position, glm::vec3 direction) : Entity(shader(), "BALL1.obj") {
		this->position = position;
		velocity = direction * glm::vec3(50.0f);
	}
};
