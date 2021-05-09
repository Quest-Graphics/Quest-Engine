#pragma once

#include <glm/gtx/random.hpp>

#include "Level.h"
#include "Enemy.h"

class DemoLevel : public Level
{
public:
	std::vector<Enemy*> enemies;

	DemoLevel()
	{
		Shader* sceneShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
		addEntity(mainStage = new Entity(sceneShader, "STAGE2.obj"));
		
		for (int i = 0; i < 1; i++) { // TODO: increase
			auto enemy = new Enemy(glm::compRand3(0.0f, mainStage->_model->size().x));
			enemies.push_back(enemy);
			addEntity(enemy);
		}
	}

	std::string name()
	{
		return "Demo Level";
	}

	void score() {
		static int score = 0;
		glutSetWindowTitle(("Score: " + std::to_string(++score)).c_str());
	}
};
