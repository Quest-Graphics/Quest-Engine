#pragma once

#include "tiny_obj_loader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"

typedef long entity_id_t;

static std::map<std::string, Model> modelCache;
static entity_id_t currentId = 0;

class Entity {
private:
	entity_id_t _id;
	Model* _model = nullptr;
	Shader* _shader = nullptr;

public:
	Entity(Shader* shader, std::string initialModelFile = "")
	{
		_id = ++currentId;

		if (!initialModelFile.empty())
		{
			setModel(initialModelFile);
		}

		_shader = shader;
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

	void render(struct CameraView* view)
	{
		if (_model)
		{
			_model->render(view, _shader);
			checkError(("Entity(" + std::to_string(_id) + ")::render").c_str());
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

		for (int i = 0; i < model.shapes.size(); i++) {
			model.m_VBO.push_back(0);
			model.m_NBO.push_back(0);
			model.m_IBO.push_back(0);
			glGenBuffers(1, &model.m_VBO[i]); // generate a 'name' for the VBO
			glGenBuffers(1, &model.m_NBO[i]); // generate a 'name' for the NBO
			glGenBuffers(1, &model.m_IBO[i]); // generate a 'name' for the IBO
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
