#pragma once

#include <playsoundapi.h>

#include "Entity.h"
#include "Shader.h"
#include "DemoLevel.h"

class Coin : public Entity {
private:
	DemoLevel* _level;

public:
	bool collected = false;

	Coin(Shader* shader, DemoLevel * level) : Entity(shader, "COIN1.obj") {
		_level = level;
		position.y = 1.0f;
	}

	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
		if (collected) {
			return;
		}
		
		Entity::render(model, view, projection);
	}

	void hit() {
		if (collected) {
			return;
		}

		_level->score();
		collected = true;
	}
};
