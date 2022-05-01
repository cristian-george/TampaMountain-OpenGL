#pragma once

#include <GLM.hpp>
#include <gtc/type_ptr.hpp>
#include <glfw3.h>

class Camera
{
public:
	enum class CameraMovementType
	{
		UNKNOWN,
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

public:
	Camera(const int width, const int height, const glm::vec3& position);

	void Set(const int width, const int height, const glm::vec3& position);
	void Reset(const int width, const int height);
	void Reshape(int windowWidth, int windowHeight);

	const glm::mat4 GetViewMatrix() const;
	const glm::mat4 GetProjectionMatrix() const;

	void MouseControl(float xPos, float yPos);

	void ProcessKeyboard(CameraMovementType direction, float deltaTime);
	void ProcessMouseScroll(float yOffset);

private:
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateCameraVectors();

private:
	const float zNEAR = 0.1f;
	const float zFAR = 500.f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float FOV = 45.0f;
	glm::vec3 startPosition;

protected:
	const float cameraSpeedFactor = 2.5f;
	const float mouseSensitivity = 0.1f;

protected:
	int width;
	int height;
	glm::vec3 position;

	float zNear;
	float zFar;
	float FoVy;
	bool isPerspective;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

protected:
	float yaw;
	float pitch;

	bool bFirstMouseMove = true;
	float lastX = 0.0f, lastY = 0.0f;
};