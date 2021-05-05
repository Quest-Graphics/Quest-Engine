#include "Entity.h"

entity_id_t Entity::currentId = 0;

Entity::Entity(Shader* shader, std::string initialModelFile) {
	_id = ++currentId;

	if (!initialModelFile.empty())
	{
		setModel(initialModelFile);
	}

	_shader = shader;

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	facing = 0;
}

entity_id_t Entity::id()
{
	return _id;
}

void Entity::render(glm::mat4 view, glm::mat4* projection)
{
	if (_model)
	{
		_model->render(view * glm::translate(position) * glm::rotate(facing, AXIS_Y), projection, _shader);
		checkError(("Entity(" + std::to_string(_id) + ")::render").c_str());
	}
}

bool Entity::setModel(std::string objectFile) {
	Model* model = Model::getOrLoad(objectFile);

	if (model) {
		this->_model = model;
		this->boundingBox = model->extent();
		return true;
	}

	return false;
}

AABB Entity::extent() {
	return this->_model->extent();
}
