#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum direction {
	UP, DOWN, LEFT, RIGHT
};

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, glm::vec3 right, float fov, float sensitivity, float mvmtSpeed);

	float m_sensitivity = 500.0f;
	float m_mvmtSpeed = 100.0f;

	float m_pitch = 0.0f; //vertical rotation angle
	float m_yaw = -90.0f; //horizontal rotation angle
	float m_fov = 45.0; //field of vision
	
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;

	glm::mat4 viewMatrix();
	void moveCamera(direction dir, float deltaTime);
	void keyRotate(direction dir, float deltaTime);
	void mouseRotate(float deltaX, float deltaY, float deltaTime);
	void lookAt(glm::vec3 target);
	void moveWithPlayer(direction dir, float MOVE_DIST);
};
