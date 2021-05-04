#pragma once

#include "Level.h"
#include "Scene.h"

class DemoLevel : public Level
{
public:
	DemoLevel()
	{
		Shader* sceneShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
		addEntity(new Scene(sceneShader, "STAGE1.obj"));
	}

	std::string name()
	{
		return "Demo Level";
	}
};
