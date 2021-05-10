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
		if (!setTexture(initialTextureFile))
			printf("Error loading texture %s", initialTextureFile.c_str());
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
		if (!_texture)
			_model->render(model * glm::translate(position) * glm::rotate(facing, AXIS_Y), view, projection, _shader);
		else
			_model->render(model * glm::translate(position) * glm::rotate(facing, AXIS_Y), view, projection, _shader, _texture);
		
		(("Entity(" + std::to_string(_id) + ")::render").c_str());
	}
}

//sets the entity's model to the model contained in objectFile- returns true if successful, false if not
bool Entity::setModel(std::string objectFile) {
	Model* model = Model::getOrLoad(objectFile);

	if (model) {
		this->_model = model;
		return true;
	}

	return false;
}

//sets the entitity's texture to the texture contained in textureFile- returns true if successful, false if not
bool Entity::setTexture(std::string textureFile)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(("Models/"+textureFile).c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	if (data)
	{

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		_texture = texture;
		return true;
	}
	return false;
}

//generates the entity's bounding box
AABB Entity::extent() {
	return _model->extent() + position;
}
