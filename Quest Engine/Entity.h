#pragma once

#include <iostream>

#include "tiny_obj_loader.h"

typedef long entity_id_t;

struct Model
{
	std::string file;
	struct tinyobj::attrib_t attributes;
	std::vector<struct tinyobj::shape_t> shapes;
	std::vector<struct tinyobj::material_t> materials;
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
			bool success = tinyobj::LoadObj(&model.attributes, &model.shapes, &model.materials, &warn, &err, objectFile.c_str());

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
};
