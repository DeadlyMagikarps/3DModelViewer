#include "Camera.h"

Camera::Camera()
{
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_position_coords, glm::vec3(0.0, 0.0, 0.0), m_up_vec);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(m_fov), (float)width / (float)height, 0.1f, 100.0f);
}

void Camera::Orbit(float x_offset, float y_offset)
{
	m_position_xangle += x_offset * m_mouse_sensitivity;
	m_position_yangle -= y_offset * m_mouse_sensitivity;

	while (m_position_xangle >= 360.0)
	{
		m_position_xangle -= 360.0;
	}
	while (m_position_xangle <= 0.0)
	{
		m_position_xangle += 360.0;
	}
	if (m_position_yangle >= 90.0)
	{
		m_position_yangle = 89.0;
	}
	if (m_position_yangle <= -90.0)
	{
		m_position_yangle = -89.0;
	}

	updateCameraVectors();
}

void Camera::Zoom(double y_offset)
{
	m_distance -= y_offset * m_zoom_sensitivity;

	if (m_distance < 1.0)
	{
		m_distance = 1.0;
	}

	updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		m_position_coords += Front * velocity;
	if (direction == BACKWARD)
		m_position_coords -= Front * velocity;
	if (direction == UP)
		m_position_coords += Front * velocity;
	if (direction == DOWN)
		m_position_coords -= Front * velocity;
	if (direction == LEFT)
		m_position_coords -= Right * velocity;
	if (direction == RIGHT)
		m_position_coords += Right * velocity;
}

void Camera::updateCameraVectors()
{
	m_position_coords = glm::vec3(
		glm::cos(glm::radians(m_position_xangle)) * glm::cos(glm::radians(m_position_yangle)),
		glm::sin(glm::radians(m_position_yangle)),
		glm::sin(glm::radians(m_position_xangle)) * glm::cos(glm::radians(m_position_yangle))
	);

	m_position_coords *= glm::vec3(m_distance);
	glm::vec3 front_vec = glm::normalize(m_position_coords) * glm::vec3(-1.0);
	glm::vec3 right_vec = glm::normalize(glm::cross(front_vec, m_WORLD_UP_VEC));
	m_up_vec = glm::normalize(glm::cross(right_vec, front_vec));
}

