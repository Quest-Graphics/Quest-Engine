#pragma once

#include "Entity.h"

/** An entity that does not interact with the player beyond collisions. */
class Scene : public Entity
{
public:
	Scene(std::string initialModelFile = "") : Entity(initialModelFile) {
	}
};
