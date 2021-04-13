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
		position = glm::vec3(100.0f, 100.0f, 100.0f);
	}

	struct CameraView view() {
		return {
			glm::lookAt(position, center, up),
			projection,
		};
	}

protected:
	const glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::mat4 projection = glm::perspective(10.0f, 1.0f, 1.0f, 1000.0f);

	glm::vec3 position;
};
