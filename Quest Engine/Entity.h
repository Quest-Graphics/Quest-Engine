#pragma once

#include "tiny_obj_loader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Shader.h"
#include "Model.h"

typedef long entity_id_t;

const glm::vec3 AXIS_Y(0.0f, 1.0f, 0.0f);

/**
* A game entity that can be interacted with and optionally rendered (if a model is provided).
*/
struct Entity {
public:
	glm::vec3 position;
	glm::vec3 velocity;
	float facing; // degrees

	/**
	* Create an entity with a given shader, 3D model filename (optional), and texture filename (optional).
	*/
	Entity(Shader* shader, std::string initialModelFile = "", std::string initialTextureFile = "");

	/**
	* Get this entity's unique ID.
	*/
	entity_id_t id();

	/**
	* Render the entity to the screen (if this entity has a model loaded)
	*/
	virtual void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

	/**
	 * Open an object file and read it into memory.
	 * If there is an error, the returned Model will have its `loaded` field set to false.
	 * If this object file has been successfully loaded before, it will not be reopened,
	 * and the old data will be returned instead. This speeds up loading many instances
	 * of the same model.
	 */
	bool setModel(std::string objectFile);
	bool setTexture(std::string textureFile);

	/**
	* Get this entity's axis-aligned bounding box (if a model is loaded).
	*/
	AABB extent();

	/**
	* Check whether this entity intersects another entity.
	*/
	bool intersects(Entity* other) {
		const AABB myBB = extent();
		const AABB otherBB = other->extent();

		return myBB.minX < otherBB.maxX && myBB.maxX > otherBB.minX
			&& myBB.minY < otherBB.maxY && myBB.maxY > otherBB.minY
			&& myBB.minZ < otherBB.maxZ && myBB.maxZ > otherBB.minZ;
	}

protected:
	static entity_id_t currentId; // The current state of the ID sequence

	unsigned int _texture = NULL;
	entity_id_t _id; // The entity's unique ID
	Model* _model = nullptr;
	Shader* _shader = nullptr;
};
