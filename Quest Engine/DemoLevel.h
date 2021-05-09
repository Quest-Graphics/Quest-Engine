#pragma once

#include <playsoundapi.h>

#include "Level.h"

class DemoLevel : public Level
{
public:
	DemoLevel()
	{
		Shader* sceneShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
		addEntity(mainStage = new Entity(sceneShader, "STAGE2.obj"));
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
		glutSetWindowTitle(("Score: " + std::to_string(++score)).c_str());
	}
};
