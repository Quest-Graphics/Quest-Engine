#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>

struct CameraView {
	glm::mat4 view;
	glm::mat4 projection;
};

class Camera
{
public:
	Camera() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		projection = glm::perspective(45.0f, 1.0f, 1.0f, 1000.0f);
	}

	struct CameraView view() {
		return {
			glm::mat4(1.0f),
			projection,
		};
	}

protected:
	glm::vec3 position;
	glm::mat4 projection;
};
