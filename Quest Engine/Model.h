#pragma once

#include "Camera.h"

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

	void render(struct CameraView* view, Shader* shader)
	{
		shader->use();

		shader->setMat4("projection", view->projection);
		shader->setMat4("modelView", view->view);
		//shader->setVec4("lightPosition", glm::vec4(1.0, 0.0, 0.0, 1.0));

		for (int i = 0; i < shapes.size(); i++) {
			// VBO
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
			glEnableVertexAttribArray((*shader)["vertexPosition"]);
			glVertexAttribPointer((*shader)["vertexPosition"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

			// NBO
			glBindBuffer(GL_ARRAY_BUFFER, m_NBO[i]);
			glEnableVertexAttribArray((*shader)["vertexNormal"]);
			glVertexAttribPointer((*shader)["vertexNormal"], 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

			// IBO
			glBindBuffer(GL_ARRAY_BUFFER, m_IBO[i]);
			glDrawElements(GL_TRIANGLES, shapes[i].mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}
	}
};
