#pragma once

#include "Level.h"

class DemoLevel : public Level
{
public:
	DemoLevel()
	{
		for (auto i = 0; i < 5; i++)
		{
			addEntity(new Entity("Models/cube.obj"));
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
