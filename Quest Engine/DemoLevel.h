#pragma once

#include "Level.h"
#include "Scene.h"

class DemoLevel : public Level
{
public:
	DemoLevel()
	{
		Shader* sceneShader = new Shader("Shaders/phong2.vert", "Shaders/phong2.frag");
		addEntity(new Scene(sceneShader, "STAGE1.obj"));
	}

	std::string name()
	{
		return "Demo Level";
	}
};
