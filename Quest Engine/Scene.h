#pragma once

#include "Entity.h"
#include "Shader.h"

/** An entity that does not interact with the player beyond collisions. */
class Scene : public Entity
{
public:
	Scene(Shader* shader, std::string initialModelFile = "") : Entity(shader, initialModelFile) {
	}
};
