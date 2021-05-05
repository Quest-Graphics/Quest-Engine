#include "Camera.h"
#include "Camera.h"

Camera::Camera()
{
	m_position = glm::vec3(15.0f, 10.0f, 10.0f);
	m_forward = glm::vec3(-0.7f, -0.5f, -1.0f);
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
		m_position += m_forward * stepSize;
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

void Camera::moveWithPlayer(direction dir, float MOVE_DIST)
{
	switch (dir)
	{
		case UP:
			m_position.z -= MOVE_DIST;
			break;
		case DOWN:
			m_position.z += MOVE_DIST;
			break;
		case RIGHT:
			m_position.x += MOVE_DIST;
			break;
		case LEFT:
			m_position.x -= MOVE_DIST;
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
		m_pitch += stepSize;
		if (m_pitch >= 89.0) m_pitch = 89.0;
		rotate = glm::rotate(rotate, -stepSize, m_right);

		m_forward = glm::vec3(glm::vec4(m_forward, 1.0f) * rotate);
		m_up = glm::vec3(glm::vec4(m_up, 1.0f) * rotate);
		break;

	case DOWN:
		m_pitch -= stepSize;
		if (m_pitch <= -89.0) m_pitch = -89.0;
		rotate = glm::rotate(rotate, stepSize, m_right);

		m_forward = glm::vec3(glm::vec4(m_forward, 1.0f) * rotate);
		m_up = glm::vec3(glm::vec4(m_up, 1.0f) * rotate);
		break;

	case LEFT:
		m_yaw += stepSize;
		rotate = glm::rotate(rotate, -stepSize, m_up);

		m_forward = glm::vec3(glm::vec4(m_forward, 1.0f) * rotate);
		m_right = glm::vec3(glm::vec4(m_right, 1.0f) * rotate);
		break;

	case RIGHT:
		m_yaw -= stepSize;
		rotate = glm::rotate(rotate, stepSize, m_up);

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


//leaves camera in the same position, turns it towards target
void Camera::lookAt(glm::vec3 target)
{
	//reset camera vectors
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::vec3(1.0f, 0.0f, 0.0f);

	//direction camera will be looking at
	glm::vec3 targetDir = glm::normalize(target - m_position);

	//used to calculate pitch and yaw
	glm::vec3 targetRef = glm::vec3(targetDir.x, 0.0f, targetDir.z);
	glm::vec3 forwardRef = glm::vec3(m_forward.x, 0.0f, m_forward.z);

	//rotates the camera horizontally
	m_yaw = acos(glm::dot(glm::normalize(targetRef), glm::normalize(forwardRef)));

	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));

	m_forward = glm::vec3(glm::vec4(m_forward, 1.0f) * rotate);
	m_right = glm::vec3(glm::vec4(m_right, 1.0f) * rotate);

	//rotates camera vertically
	m_pitch = acos(glm::dot(glm::normalize(m_forward), glm::normalize(targetDir)));

	if (m_pitch >= 89.0f) m_pitch = 89.0f;
	if (m_pitch <= -89.0f) m_pitch = -89.0f;

	rotate = glm::rotate(glm::mat4(1.0f), m_pitch, m_right);

	m_forward = glm::vec3(glm::vec4(m_forward, 1.0f) * rotate);
	m_up = glm::vec3(glm::vec4(m_up, 1.0f) * rotate);
}
