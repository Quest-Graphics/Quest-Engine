#pragma once

#include <playsoundapi.h>

#include "Level.h"

class DemoLevel : public Level
{
public:
	static const int NUM_COINS = 10;

	DemoLevel()
	{
		Shader* sceneShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
		addEntity(mainStage = new Entity(sceneShader, "STAGE2.obj", "STAGE.tga"));
		PlaySound(L"Sounds/DOOM 2016.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}

	~DemoLevel() {
		PlaySound(NULL, 0, 0);
	}

	std::string name()
	{
		return "Demo Level";
	}

	void score() {
		static int score = 0;

		if (++score == 10) {
			glutSetWindowTitle("You win!");
			std::cout << "You Win!" << std::endl;
			exit(0);
		}
		else {
			glutSetWindowTitle(("Score: " + std::to_string(score) + "/" + std::to_string(NUM_COINS)).c_str());
		}
	}
};
