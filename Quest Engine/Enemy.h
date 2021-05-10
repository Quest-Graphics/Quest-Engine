#pragma once

#include "Entity.h"
#include "Projectile.h"

class Enemy : public Entity {
protected:
	Shader* _shader;
	float lastShot;
	float fireRate;

	// Initialize shader after GL
	Shader* shader() {
		if (!_shader) {
			_shader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
		}

		return _shader;
	}

	std::vector<Projectile*> projectiles;

public:

	Enemy(glm::vec3 position, float fireRate) : Entity(shader(), "cube.obj") {
		this->position = position;
		this->lastShot = 0.0f;
		this->fireRate = fireRate;
	}

	//fires shots at fireRate
	bool fire(float currentTime)
	{
		if (currentTime - lastShot >= fireRate || lastShot == 0)
		{
			lastShot = currentTime;
			return true;
		}
		return false;
	}

	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override {
		Entity::render(model, view, projection);
	}
};
