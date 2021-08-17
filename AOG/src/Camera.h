#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

// Const variables
const float YAW			= -90.0f;
const float PITCH		= 0.0f;
const float SPEED		= 2.5f;
const float SENSITIVITY	= 0.1f;
const float ZOOM		= 45.0f;

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{

private:
	glm::vec3 m_Position;		// Position Vector of the camera
	glm::vec3 m_Front;			// Z direction vector
	glm::vec3 m_Up;				// Y direction vector
	glm::vec3 m_Right;			// X direction vector
	glm::vec3 m_WorldUp;		// World Y direction vector

	float m_Pitch;				// Euler angle changing view on Y / (X/Z) axis
	float m_Yaw;				// Euler angle changing view on Z / X axis

	float m_MovementSpeed;		// Camera Options
	float m_MouseSensitivity;
	float m_Zoom;

public:
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f))
		: m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Pitch(PITCH), m_Yaw(YAW), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM) 
	{
		m_Position = position;
		m_WorldUp = worldUp;

		updateCameraVectors();
	}

	/* The target here is m_Front which is the Z axis */
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(m_Position, m_Position + m_Front, m_Up); 
	}

	void ProcessKeyboard(CameraMovement mov, float deltaTime){
		float velocity = deltaTime * m_MovementSpeed;
		switch (mov) {
			case CameraMovement::FORWARD:
				m_Position += m_Front * velocity;
				break;
			case CameraMovement::BACKWARD:
				m_Position -= m_Front * velocity;
				break;
			case CameraMovement::LEFT:
				m_Position -= m_Right * velocity;
				break;
			case CameraMovement::RIGHT:
				m_Position += m_Right * velocity;
				break;
		}
	}

	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		yoffset *= m_MouseSensitivity;
		xoffset *= m_MouseSensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		// Constraints
		if (m_Pitch > 89.0f) {
			m_Pitch = 89.0f;
		}
		else if (m_Pitch < -89.0f) {
			m_Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler 
		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset) {
		m_Zoom -= (float)yoffset;
		if (m_Zoom < 1.0f) {
			m_Zoom = 1.0f;
		}	
		if (m_Zoom > 45.0f) {
			m_Zoom = 45.0f;
		}
	}

	// Getters and Setters
	const float GetZoom() const { return m_Zoom; }
	const glm::vec3& GetPosition() const { return m_Position; }

private:
	void updateCameraVectors() {
		glm::vec3 front;

		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)); // yaw z/x plane
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		
		m_Front = glm::normalize(front);

		// Re-calculate the right and up vectors
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}
};