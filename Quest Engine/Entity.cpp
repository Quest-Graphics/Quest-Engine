#include "Entity.h"
#include "stb_image.h"

entity_id_t Entity::currentId = 0;

Entity::Entity(Shader* shader, std::string initialModelFile, std::string initialTextureFile) {
	_id = ++currentId;

	if (!initialModelFile.empty())
	{
		setModel(initialModelFile);
	}

	if (!initialTextureFile.empty())
	{
		setTexture(initialTextureFile);
	}
	_shader = shader;

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	facing = 0;
}

entity_id_t Entity::id()
{
	return _id;
}

void Entity::render(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	if (_model)
	{
		_model->render(model * glm::translate(position) * glm::rotate(facing, AXIS_Y), view, projection, _shader);
		
		
		(("Entity(" + std::to_string(_id) + ")::render").c_str());
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

bool Entity::setTexture(std::string textureFile)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);

	return false;
}

AABB Entity::extent() {
	return _model->extent() + position;
}
