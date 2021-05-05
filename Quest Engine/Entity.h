#pragma once

#include "tiny_obj_loader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Shader.h"
#include "Model.h"

typedef long entity_id_t;

const glm::vec3 AXIS_Y(0.0f, 1.0f, 0.0f);

struct Entity {
	Entity(Shader* shader, std::string initialModelFile = "");

	AABB boundingBox;

	entity_id_t id();

	void render(glm::mat4 view, glm::mat4* projection);

	/**
	 * Open an object file and read it into memory.
	 * If there is an error, the returned Model will have its `loaded` field set to false.
	 * If this object file has been successfully loaded before, it will not be reopened,
	 * and the old data will be returned instead. This speeds up loading many instances
	 * of the same model.
	 */
	bool setModel(std::string objectFile);

protected:
	static entity_id_t currentId;

	entity_id_t _id;
	Model* _model = nullptr;
	Shader* _shader = nullptr;
	glm::vec3 position;
	float facing; // degrees
};
