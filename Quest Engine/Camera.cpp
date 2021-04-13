#include "Camera.h"

Camera::Camera()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::vec3(1.0f, 0.0f, 0.0f);
}

//constructor
Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, glm::vec3 right, float fov, float sensitivity, float mvmtSpeed)
{
	m_position = position;
	m_forward = glm::normalize(forward);
	m_up = glm::normalize(up);
	m_right = glm::normalize(right);

	m_fov = fov;
	m_sensitivity = sensitivity;
	m_mvmtSpeed = mvmtSpeed;
}


glm::mat4 Camera::viewMatrix()
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

//moves the camera based on the key pressed
void Camera::moveCamera(direction dir, float deltaTime)
{
	float stepSize = m_mvmtSpeed * deltaTime;

	switch (dir)
	{
	case UP:
		m_position +=  m_forward * stepSize;
		break;

	case DOWN:
		m_position -= m_forward * stepSize;
		break;

	case LEFT:
		m_position -= m_right * stepSize;
		break;

	case RIGHT:
		m_position += m_right * stepSize;
		break;
	}
}

//rotates the camera based on a key press/direction given
void Camera::keyRotate(direction dir, float deltaTime)
{
	glm::mat4 rotate = glm::mat4(1.0f);
	float stepSize = m_sensitivity * deltaTime;

	switch (dir)
	{
	case UP:
		if(m_pitch + stepSize >= 90)
			m_pitch += stepSize;
		rotate = glm::rotate(rotate, stepSize, m_right);

		m_forward = glm::vec3(glm::vec4(m_forward, 1.0f) * rotate);
		m_up = glm::vec3(glm::vec4(m_up, 1.0f) * rotate);
		break;

	case DOWN:
		if(m_pitch + stepSize <= 90)
			m_pitch -= stepSize;
		rotate = glm::rotate(rotate, -stepSize, m_right);

		m_forward = glm::vec3(glm::vec4(m_forward, 1.0f) * rotate);
		m_up = glm::vec3(glm::vec4(m_up, 1.0f) * rotate);
		break;

	case LEFT:
		m_yaw += stepSize;
		rotate = glm::rotate(rotate, stepSize, m_up);

		m_forward = glm::vec3(glm::vec4(m_forward, 1.0f) * rotate);
		m_right = glm::vec3(glm::vec4(m_right, 1.0f) * rotate);
		break;

	case RIGHT:
		m_yaw -= stepSize;
		rotate = glm::rotate(rotate, -stepSize, m_up);

		m_forward = glm::vec3(glm::vec4(m_forward, 1.0f) * rotate);
		m_right = glm::vec3(glm::vec4(m_right, 1.0f) * rotate);
		break;
	}

	m_forward = glm::normalize(m_forward);
	m_up = glm::normalize(m_up);
	m_right = glm::normalize(m_right);
}

//WIP
//Moves when the mouse cursor moves
void Camera::mouseRotate(float deltaX, float deltaY, float deltaTime)
{

}