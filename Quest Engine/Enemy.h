#pragma once

#include "Entity.h"
#include "Projectile.h"

class Enemy : public Entity {
protected:
	Shader* _shader;

	// Initialize shader after GL
	Shader* shader() {
		if (!_shader) {
			_shader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
		}

		return _shader;
	}

	std::vector<Projectile*> projectiles;
	unsigned short lastShot = 0; // how long ago we last shot a projectile

public:
	Enemy(glm::vec3 position) : Entity(shader(), "cube.obj") {
		this->position = position;
	}

	void tick() {
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
	}

	void render(glm::mat4 view, glm::mat4* projection) override {
		Entity::render(view, projection);

		// Render projectiles
		for (auto projectile : projectiles) {
			projectile->render(view, projection);
		}
	}

	void shoot() {
		projectiles.push_back(new Projectile(position, facing_vec()));
		lastShot = 0;
	}
};
