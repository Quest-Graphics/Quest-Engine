#pragma once

#include <string>

#include "Entity.h"

class Level {
public:
	virtual std::string name()
	{
		return "Unnamed Level";
	}

	void render(struct CameraView* view)
	{
		for (auto& entity : entities)
		{
			entity.second->render(view);
		}
	}

protected:
	std::map<entity_id_t, Entity*> entities;

	void addEntity(Entity* entity)
	{
		entities[entity->id()] = entity;
	}
};
