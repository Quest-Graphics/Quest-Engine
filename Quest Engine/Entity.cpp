#include "Entity.h"

entity_id_t Entity::currentId = 0;

Entity::Entity(Shader* shader, std::string initialModelFile) {
	_id = ++currentId;

	if (!initialModelFile.empty())
	{
		setModel(initialModelFile);
	}

	_shader = shader;
}

void Entity::render(glm::mat4* view, glm::mat4* projection)
{
	if (_model)
	{
		_model->render(view, projection, _shader);
		checkError(("Entity(" + std::to_string(_id) + ")::render").c_str());
	}
}

bool Entity::setModel(std::string objectFile) {
	Model* model = Model::getOrLoad(objectFile);

	if (model) {
		this->_model = model;
		return true;
	}

	return false;
}
