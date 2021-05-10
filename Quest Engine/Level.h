#pragma once

#include <string>

#include "Entity.h"

/**
* A level of a game.
* Manages entities and events between them.
*/
class Level {
public:
	/**
	* The "world" entity that serves as a ground plane
	*/
	Entity* mainStage;

	/**
	* Remove this level and all entities in it.
	*/
	~Level()
	{
		for (std::map<entity_id_t, Entity*>::iterator i = entities.begin(); i != entities.end(); i++)
		{
			delete i->second;
		}
	}

	virtual std::string name()
	{
		return "Unnamed Level";
	}

	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
	{
		for (auto& entity : entities)
		{
			entity.second->render(model, view, projection);
		}
	}

protected:
	std::map<entity_id_t, Entity*> entities;

	void addEntity(Entity* entity)
	{
		entities[entity->id()] = entity;
	}
};
