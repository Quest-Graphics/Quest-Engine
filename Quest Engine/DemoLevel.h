#pragma once

#include "Level.h"
#include "Scene.h"

class DemoLevel : public Level
{
public:
	DemoLevel()
	{
		Shader* sceneShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");

		for (auto i = 0; i < 5; i++)
		{
			addEntity(new Scene(sceneShader, "STAGE1.obj"));
		}
	}

	~DemoLevel()
	{
		for (std::map<entity_id_t, Entity*>::iterator i = entities.begin(); i != entities.end(); i++)
		{
			delete i->second;
		}
	}

	std::string name()
	{
		return "Demo Level";
	}
};
