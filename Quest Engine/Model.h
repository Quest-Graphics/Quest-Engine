#pragma once

#include "Camera.h"
#include "Shader.h"
#include "util.h"
#include <vector>
#include <map>
#include "tiny_obj_loader.h"

struct Model
{
	static Model* getOrLoad(std::string name);

	std::string file;
	bool loaded = false;
	
	struct tinyobj::attrib_t attributes;
	std::vector<struct tinyobj::shape_t> shapes;
	std::vector<struct tinyobj::material_t> materials;
	
	GLuint vbo, nbo;
	std::vector<GLuint> m_IBO;	// index buffer IDs, each corresponding to a shape

	/**
	* Load this model into the GPU's buffers.
	*/
	void buffer();

	/**
	* Render this model to the screen.
	* buffer() must have been called previously if another model was rendered last.
	*/
	void render(glm::mat4* view, glm::mat4* projection, Shader* shader);

private:
	static std::map<std::string, Model> modelCache;
};
