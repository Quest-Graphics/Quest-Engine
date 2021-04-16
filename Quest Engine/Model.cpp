#include "Model.h"

std::map<std::string, Model> Model::modelCache;

Model* Model::getOrLoad(std::string name) {
	Model& model = modelCache[name];

	if (!model.loaded)
	{
		std::cout << "Loading new model " << name << std::endl;
		std::string warn, err;
		bool success = tinyobj::LoadObj(
			&model.attributes, &model.shapes, &model.materials, // Load output
			&warn, &err,								        // Status output
			("Models/" + name).c_str(), "Models"                // Input locations
		);

		if (!warn.empty())
		{
			std::cerr << warn << std::endl;
		}

		if (!err.empty())
		{
			std::cerr << err << std::endl;
		}

		if (!success) {
			std::cerr << "Failed to load model " << name << std::endl;
			return nullptr;
		}

		// Set fields in model object
		model.file = name;
		model.loaded = true;

		// Generate 'names' for the VBO and NBO
		glGenBuffers(1, &model.vbo);
		glGenBuffers(1, &model.nbo);

		// Generate 'name' for the IBOs (one per shape)
		model.m_IBO.resize(model.shapes.size());
		glGenBuffers(model.shapes.size(), &model.m_IBO[0]);
		model.buffer();

		// Store in cache for next time
		modelCache[name] = model;
	}
	else {
		std::cout << "Using cached model " << name << std::endl;
	}

	return &model;
}

void Model::buffer() {
	std::cout << "Buffering model " << file << std::endl;

	// Tell OpenGL we want to use the VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Fill the VBO with vertex data
	glBufferData(GL_ARRAY_BUFFER, attributes.vertices.size() * sizeof(float), &attributes.vertices[0], GL_STATIC_DRAW);

	// Tell OpenGL we want to use the NBO
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	// Fill the NBO with normal data
	glBufferData(GL_ARRAY_BUFFER, attributes.normals.size() * sizeof(float), &attributes.normals[0], GL_STATIC_DRAW);

	for (size_t i = 0; i < shapes.size(); i++)
	{	
		// Tell OpenGL we want to use the IBO for this shape
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[i]);
		// Fill IBO with index data for this shape
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), &shapes[i].mesh.indices[0], GL_STATIC_DRAW);
	}
}

void Model::render(glm::mat4* view, glm::mat4* projection, Shader* shader) {
	shader->use(); // Bind shader

	shader->setMat4("projection", *projection); // send projection to vertex shader
	shader->setMat4("modelView", *view);		// send modelview to vertex shader

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(ATTRLOC_vertexPosition);
	glVertexAttribPointer(ATTRLOC_vertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	checkError("Model::render/VBO");

	// NBO
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glEnableVertexAttribArray(ATTRLOC_vertexNormal);
	glVertexAttribPointer(ATTRLOC_vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	checkError("Model::render/NBO");

	// IBOs (one per shape)
	for (size_t i = 0; i < shapes.size(); i++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[i]);
		glDrawElements(GL_TRIANGLES, shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, 0);
		checkError("Model::render/IBO");
	}

	shader->unuse();
}
