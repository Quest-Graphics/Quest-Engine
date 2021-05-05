#include "Model.h"

std::map<std::string, Model> Model::modelCache;

Model* Model::getOrLoad(std::string name) {
	Model& model = modelCache[name];

	if (!model.loaded)
	{
		std::cout << "Loading new model " << name << std::endl;
		std::string err = tinyobj::LoadObj(model.shapes, ("Models/" + name).c_str(), "Models/");

		if (!err.empty())
		{
			std::cerr << "Failed to load model " << name << ": " << err << std::endl;
			return nullptr;
		}

		// Set fields in model object
		model.file = name;
		model.loaded = true;

		size_t numShapes = model.shapes.size();

		// Generate 'names' for the VBOs (one per shape)
		model.m_VBO.resize(numShapes);
		glGenBuffers(numShapes, &model.m_VBO[0]);

		// Generate 'names' for the NBOs (one per shape)
		model.m_NBO.resize(numShapes);
		glGenBuffers(numShapes, &model.m_NBO[0]);

		// Generate 'names' for the IBOs (one per shape)
		model.m_IBO.resize(model.shapes.size());
		glGenBuffers(numShapes, &model.m_IBO[0]);

		// Load buffer object data into buffers
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
	for (size_t i = 0; i < shapes.size(); i++)
	{	
		tinyobj::mesh_t * mesh = &shapes[i].mesh;

		// Tell OpenGL we want to use the VBO for this shape
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
		// Fill the VBO with vertex data
		glBufferData(GL_ARRAY_BUFFER, mesh->positions.size() * sizeof(float), &mesh->positions[0], GL_STATIC_DRAW);

		// Tell OpenGL we want to use the NBO for this shape
		glBindBuffer(GL_ARRAY_BUFFER, m_NBO[i]);
		// Fill the NBO with normal data
		glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(float), &mesh->normals[0], GL_STATIC_DRAW);

		// Tell OpenGL we want to use the IBO for this shape
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[i]);
		// Fill IBO with index data for this shape
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);
	}
}

void Model::render(glm::mat4 view, glm::mat4* projection, Shader* shader) {
	shader->use(); // Bind shader

	shader->setMat4("projection", *projection); // send projection to vertex shader
	shader->setMat4("modelView", view);		// send modelview to vertex shader

	for (size_t i = 0; i < shapes.size(); i++) {
		// VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
		glEnableVertexAttribArray(ATTRLOC_vertexPosition);
		glVertexAttribPointer(ATTRLOC_vertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		checkError("Model::render/VBO");

		// NBO
		glBindBuffer(GL_ARRAY_BUFFER, m_NBO[i]);
		glEnableVertexAttribArray(ATTRLOC_vertexNormal);
		glVertexAttribPointer(ATTRLOC_vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		checkError("Model::render/NBO");

		// IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO[i]);
		glDrawElements(GL_TRIANGLES, shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, 0);
		checkError("Model::render/IBO");
	}

	shader->unuse();
}

AABB Model::extent()
{
	// X, Y, Z
	float min[3] = { 0, 0, 0 };
	float max[3] = { 0, 0, 0 };

	if (shapes.empty()) {
		fputs("Cannot calculate extent of model with no shapes.", stderr);
	}
	else {
		// Starting values
		for (int axis = 0; axis < 3; axis++) {
			min[axis] = max[axis] = shapes[0].mesh.positions[axis];
		}

		// Check all shapes in this model
		for (auto& shape : shapes) {
			// Check all vertices in this shape
			for (int p = 0; p < shape.mesh.positions.size();) {
				// Update all axes from this vertex
				for (int axis = 0; axis < 3; axis++) {
					float* position = &shape.mesh.positions[p];

					if (*position < min[axis]) {
						min[axis] = *position;
					}

					if (*position > max[axis]) {
						max[axis] = *position;
					}

					p++; // Next axis position is at the next element of positions 
				}
			}
		}
	}

	AABB boundingBox = {
		min[0], max[0], // X
		min[1], max[1], // Y
		min[2], max[2], // Z
	};

	printf("Computed model extents:\n\tminX = %+f\tmaxX = %+f\n\tminY = %+f\tmaxY = %+f\n\tminZ = %+f\tmaxZ = %+f\n",
		boundingBox.minX, boundingBox.maxX,
		boundingBox.minY, boundingBox.maxY,
		boundingBox.minZ, boundingBox.maxZ);

	return boundingBox;
}
