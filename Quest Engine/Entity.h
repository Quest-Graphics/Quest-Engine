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
	glm::vec3 position;
	glm::vec3 velocity;
	Model* _model = nullptr;
	float facing; // degrees

	Entity(Shader* shader, std::string initialModelFile = "");

	entity_id_t id();

	virtual void render(glm::mat4 view, glm::mat4* projection);

	/**
	 * Open an object file and read it into memory.
	 * If there is an error, the returned Model will have its `loaded` field set to false.
	 * If this object file has been successfully loaded before, it will not be reopened,
	 * and the old data will be returned instead. This speeds up loading many instances
	 * of the same model.
	 */
	bool setModel(std::string objectFile);

	AABB extent();

	bool intersects(Entity* other) {
		const AABB myBB = extent();
		const AABB otherBB = other->extent();

		return (myBB.minX < otherBB.maxX) && (myBB.maxX > otherBB.minX)
			&& (myBB.minY < otherBB.maxY) && (myBB.maxY > otherBB.minY)
			&& (myBB.minZ < otherBB.maxZ) && (myBB.maxZ > otherBB.minZ);
	}

	virtual void tick() {
		position += velocity;
	}

	glm::vec3 facing_vec() {
		const float radians = glm::radians(facing);
		return glm::normalize(glm::vec3(glm::cos(radians), 0.0f, glm::sin(radians)));
	}

protected:
	static entity_id_t currentId;

	entity_id_t _id;
	Shader* _shader = nullptr;
};
