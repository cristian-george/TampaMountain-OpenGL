#include "Camera.h"

Camera::Camera(const int width, const int height, const glm::vec3& position) : startPosition(position)
{
	Set(width, height, position);
}

void Camera::Set(const int width, const int height, const glm::vec3& position)
{
	this->width = width;
	this->height = height;
	this->position = position;

	zNear = zNEAR;
	zFar = zFAR;
	FoVy = FOV;
	isPerspective = true;

	worldUp = glm::vec3(0, 1, 0);

	yaw = YAW;
	pitch = PITCH;
	bFirstMouseMove = true;

	lastX = width / 2.0f;
	lastY = height / 2.0f;

	UpdateCameraVectors();
}

void Camera::Reset(const int width, const int height)
{
	Set(width, height, startPosition);
}

void Camera::Reshape(int windowWidth, int windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	glViewport(0, 0, windowWidth, windowHeight);
}

void Camera::Use(Shader* shader)
{
	glm::mat4 projection = GetProjectionMatrix();
	glm::mat4 view = GetViewMatrix();
	shader->SetMat4("projection", projection);
	shader->SetMat4("view", view);
	shader->SetVec3("viewPos", position);
}

const glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + forward, up);
}

const glm::mat4 Camera::GetProjectionMatrix() const
{
	glm::mat4 Proj = glm::mat4(1);
	if (isPerspective)
	{
		float aspectRatio = ((float)(width)) / height;
		Proj = glm::perspective(glm::radians(FoVy), aspectRatio, zNear, zFar);
	}
	else
	{
		float scaleFactor = 2000.f;
		Proj = glm::ortho<float>(
			-width / scaleFactor, width / scaleFactor,
			-height / scaleFactor, height / scaleFactor, -zFar, zFar);
	}
	return Proj;
}

void Camera::MouseControl(float xPos, float yPos)
{
	if (bFirstMouseMove)
	{
		lastX = xPos;
		lastY = yPos;
		bFirstMouseMove = false;
	}

	float xChange = xPos - lastX;
	float yChange = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	if (fabs(xChange) <= 1e-6 && fabs(yChange) <= 1e-6)
	{
		return;
	}

	xChange *= mouseSensitivity;
	yChange *= mouseSensitivity;

	ProcessMouseMovement(xChange, yChange);
}

void Camera::ProcessKeyboard(CameraMovementType direction, float deltaTime)
{
	float velocity = (float)(cameraSpeedFactor * deltaTime);
	switch (direction)
	{
	case CameraMovementType::FORWARD:
		position += forward * velocity;
		break;
	case CameraMovementType::BACKWARD:
		position -= forward * velocity;
		break;
	case CameraMovementType::RIGHT:
		position += right * velocity;
		break;
	case CameraMovementType::LEFT:
		position -= right * velocity;
		break;
	case CameraMovementType::UP:
		position += up * velocity;
		break;
	case CameraMovementType::DOWN:
		position -= up * velocity;
		break;
	}
}

void Camera::ProcessMouseScroll(float yOffset)
{
	if (FoVy >= 1.0f && FoVy <= 90.0f)
	{
		FoVy -= yOffset;
	}

	if (FoVy <= 1.0f)
		FoVy = 1.0f;
	if (FoVy >= 90.0f)
		FoVy = 90.0f;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	this->forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->forward.y = sin(glm::radians(pitch));
	this->forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->forward = glm::normalize(this->forward);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}