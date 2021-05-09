#pragma once

#include <playsoundapi.h>

#include <glm/gtx/random.hpp>

#include "Entity.h"
#include "Shader.h"
#include "DemoLevel.h"

class Coin : public Entity {
private:
	DemoLevel* _level;

public:
	Coin(Shader* shader, DemoLevel * level) : Entity(shader, "COIN1.obj") {
		_level = level;
		randomize();
		position.y = 5.0f;
	}

	void randomize() {
		const AABB stage = _level->mainStage->extent();

		position.x = glm::compRand1(stage.minX, stage.maxX);
		position.z = glm::compRand1(stage.minZ, stage.maxZ);
	}

	void hit() {
		randomize();
		_level->score();
		PlaySound(L"Sounds/coin.wav", NULL, SND_ASYNC | SND_FILENAME);
	}
};
