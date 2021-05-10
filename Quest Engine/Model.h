#pragma once

#include "Camera.h"
#include "Shader.h"
#include "util.h"
#include <vector>
#include <map>
#include "tiny_obj_loader.h"

/* Represents the coordinates of an axis-aligned bounding box */
struct AABB {
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;

	AABB operator+(const glm::vec3 translate) {
		return {
			minX + translate.x, maxX + translate.x,
			minY + translate.y, maxY + translate.y,
			minZ + translate.z, maxZ + translate.z
		};
	}
};

struct Model
{
	static Model* getOrLoad(std::string name);

	std::string file;
	bool loaded = false;

	std::vector<tinyobj::shape_t> shapes;

	std::vector<GLuint> m_VBO;	// vertex buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_NBO;	// normal buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_IBO;	// index buffer IDs, each corresponding to a shape

	/**
	* Load this model into the GPU's buffers.
	*/
	void buffer();

	/**
	* Render this model to the screen.
	* buffer() must have been called previously if another model was rendered last.
	*/
	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Shader* shader);

	/**
		Render this model to the screen
		buffer() must have been called perviously if another model was rendered last
		For usage with a texture
	*/
	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection, Shader* shader, unsigned int texture);
	/*
	Get the extent of them model along each axis.
	@return an axis-aligned bounding box
	*/
	AABB extent();

	/**
	* Get the maximum size of the model along each axis.
	*/
	glm::vec3 size() {
		const AABB bb = extent();
		return glm::vec3(bb.maxX - bb.minX, bb.maxY - bb.minY, bb.maxZ - bb.minZ);
	}


private:
	static std::map<std::string, Model> modelCache;
};
