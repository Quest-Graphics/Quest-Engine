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

	/*
	void shoot() {
		projectiles.push_back(new Projectile(position, facing_vec()));
		lastShot = 0;
	}
	*/

	void tick() {
		/*
		// Shoot projectiles at a random interval
		if (lastShot++ >= std::rand() % 300 + 200) {
			// 200-500 ticks = 2-5 seconds
			shoot();
		}

		
		// Update all projectiles
		std::vector<Projectile*>::iterator p = projectiles.begin();
		while (p != projectiles.end()) {
			(*p)->tick();

			if (std::abs((*p)->position.x - position.x) > Projectile::MAX_RANGE ||
				std::abs((*p)->position.z - position.z) > Projectile::MAX_RANGE) {
				p = projectiles.erase(p);
			}
		}
		*/
	}

	//fires shots at fireRate
	bool fire(float currentTime)
	{
		if (currentTime - lastShot >= fireRate)
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
