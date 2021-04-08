#pragma once

#include <iostream>

#include "tiny_obj_loader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

typedef long entity_id_t;

struct Model
{
	std::string file;
	struct tinyobj::attrib_t attributes;
	std::vector<struct tinyobj::shape_t> shapes;
	std::vector<struct tinyobj::material_t> materials;
	std::vector<GLuint> m_VBO;		// vertex buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_NBO;		// normal buffer IDs, each corresponding to a shape
	std::vector<GLuint> m_IBO;		// index buffer IDs, each corresponding to a shape
	bool loaded = false;
	unsigned int refs = 0;
};

static std::map<std::string, Model> modelCache;
static entity_id_t currentId = 0;

class Entity {
private:
	entity_id_t _id;
	Model* _model = nullptr;

public:
	Entity(std::string initialModelFile = "")
	{
		_id = ++currentId;

		if (!initialModelFile.empty())
		{
			setModel(initialModelFile);
		}
	}

	~Entity()
	{
		if (_model->refs == 1)
		{
			modelCache.erase(_model->file);
		}
	}

	entity_id_t id()
	{
		return _id;
	}

	void render()
	{
		if (_model)
		{
			// TODO: render model
		}
	}

	/**
	 * Open an object file and read it into memory.
	 * If there is an error, the returned Model will have its `loaded` field set to false.
	 * If this object file has been successfully loaded before, it will not be reopened,
	 * and the old data will be returned instead. This speeds up loading many instances
	 * of the same model.
	 */
	void setModel(std::string objectFile)
	{
		Model& model = modelCache[objectFile];

		if (!model.loaded)
		{
			std::cout << "Entity " << _id << " loading new model " << objectFile << std::endl;
			std::string warn, err;
			bool success = tinyobj::LoadObj(
				&model.attributes, &model.shapes, &model.materials, // Load output
				&warn, &err,								        // Status output
				("Models/" + objectFile).c_str(), "Models"          // Input locations
			);

			if (!warn.empty())
			{
				std::cerr << warn << std::endl;
			}

			if (!err.empty())
			{
				std::cerr << err << std::endl;
			}

			if (success)
			{
				model.file = objectFile;
				model.loaded = true;
				modelCache[objectFile] = model;
				this->_model = &model;
			}
			else
			{
				std::cerr << "Entity " << _id << " failed to load model " << objectFile << std::endl;
				return;
			}
		}

		model.refs++;
		this->_model = &model;
	}

	void updateBuffers()
	{
		for (int i = 0; i < this->_model->shapes.size(); i++) 
		{
			// Tell OpenGL which VBO you want to work on right now
			glBindBuffer(GL_ARRAY_BUFFER, this->_model->m_VBO[i]);
			// Fillthe VBO with vertex data.
			glBufferData(GL_ARRAY_BUFFER, this->_model->attributes.vertices.size() * sizeof(float), &this->_model->attributes.vertices[0], GL_STATIC_DRAW);
			// Tell OpenGL which VBO you want to work on right now
			glBindBuffer(GL_ARRAY_BUFFER, this->_model->m_NBO[i]);
			// Fillthe VBO with vertex data.
			glBufferData(GL_ARRAY_BUFFER, this->_model->attributes.normals.size() * sizeof(float), &this->_model->attributes.normals[0], GL_STATIC_DRAW);
			// Bind ibo to the index buffer.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_model->m_IBO[i]);
			// Fill index buffer with index data.
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_model->shapes[i].mesh.indices.size() * sizeof(unsigned int), &this->_model->shapes[i].mesh.indices[0], GL_STATIC_DRAW);
		}
	}
};
