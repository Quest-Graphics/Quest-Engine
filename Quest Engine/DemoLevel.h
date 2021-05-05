#pragma once

#include "Level.h"

class DemoLevel : public Level
{
public:
	DemoLevel()
	{
		Shader* sceneShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
		addEntity(mainStage = new Entity(sceneShader, "STAGE2.obj"));
	}

	std::string name()
	{
		return "Demo Level";
	}
};
