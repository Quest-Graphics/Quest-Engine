#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
	}

protected:
	glm::vec3 position;
};
