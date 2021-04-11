#pragma once

#include "Entity.h"
#include "Shader.h"

static Shader* sceneShader = new Shader();

/** An entity that does not interact with the player beyond collisions. */
class Scene : public Entity
{
public:
	Scene(std::string initialModelFile = "") : Entity(sceneShader, initialModelFile) {
	}
};
