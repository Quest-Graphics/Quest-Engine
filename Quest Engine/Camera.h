#pragma once

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum direction {
	UP, DOWN, LEFT, RIGHT
};

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, glm::vec3 right, float fov, float sensitivity, float mvmtSpeed);

	float m_sensitivity = 1.0f;
	float m_mvmtSpeed = 2.0f;

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

	void onKeyboard(unsigned char key, float deltaTime)
	{
		switch (key) {
		case 'i':
		case 'I':
			moveCamera(UP, deltaTime);
			break;
		case 'k':
		case 'K':
			moveCamera(DOWN, deltaTime);
			break;
		case 'j':
		case 'J':
			moveCamera(LEFT, deltaTime);
			break;
		case 'l':
		case 'L':
			moveCamera(RIGHT, deltaTime);
			break;
		}
	}

	void onSpecialInput(int key, float deltaTime)
	{
		switch (key) {
		case GLUT_KEY_UP:
			keyRotate(UP, deltaTime);
			break;
		case GLUT_KEY_DOWN:
			keyRotate(DOWN, deltaTime);
			break;
		case GLUT_KEY_LEFT:
			keyRotate(LEFT, deltaTime);
			break;
		case GLUT_KEY_RIGHT:
			keyRotate(RIGHT, deltaTime);
			break;
		}
	}
};
