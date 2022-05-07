﻿#include <GL/glew.h>
#include <glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// timing
double deltaTime = 0.0f;    // time between current frame and last frame
double lastFrame = 0.0f;

#include "TextureLoader.h"
#include "Mesh.h"
#include "Camera.h"
Camera* pCamera;

static glm::vec3 value(0.6f);
float skyLight = value.x;

Shader shader;
Shader mapShader;

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(Camera::CameraMovementType::FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(Camera::CameraMovementType::BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(Camera::CameraMovementType::RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(Camera::CameraMovementType::LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(Camera::CameraMovementType::UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(Camera::CameraMovementType::DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);
	}
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	pCamera->Reshape(width, height);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	pCamera->MouseControl((float)xpos, (float)ypos);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yOffset)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (tolower(key) == 'd' && value.x < 0.9f)
	{
		value += glm::vec3(0.05f);
	}

	if (tolower(key) == 'n' && value.x > 0.2f)
	{
		value -= glm::vec3(0.05f);
	}

	shader.Use();
	shader.SetVec3("lightColor", value);
	mapShader.Use();
	mapShader.SetVec3("lightColor", value);

	skyLight = value.x;
}

int main()
{
	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Relief", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetKeyCallback(window, KeyCallback);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 500.0f, 300.0f));

	shader.Set("Basic.shader");
	mapShader.Set("Map.shader");

	std::filesystem::path localPath = std::filesystem::current_path();
	std::string resourcesFolder = localPath.string() + "\\Resources";

	int mapTexture = CreateTexture(resourcesFolder + "\\MapTexture\\GOOGLE_SAT_WM.jpg");
	mapShader.SetInt("mapTexture", 0);

	Mesh map("map.obj");
	map.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	map.SetPosition(glm::vec3(0.0f));
	map.InitVAO();

	shader.Use();
	shader.SetVec3("lightColor", glm::vec3(0.6f, 0.6f, 0.6f));

	mapShader.Use();
	mapShader.SetVec3("lightColor", glm::vec3(0.6f, 0.6f, 0.6f));

	while (!glfwWindowShouldClose(window))
	{
		// Per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Lighting
		float clearR = 0.07f + skyLight / 2.f - 0.1f;
		float clearG = 0.13f + skyLight / 2.f - 0.1f;
		float clearB = 0.17 + skyLight / 2.f - 0.1f;
		glClearColor(clearR, clearG, clearB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Input
		ProcessInput(window);

		// Render here
		mapShader.Use();
		pCamera->UpdateCameraVectors();
		pCamera->Use(&mapShader);
		glBindTexture(GL_TEXTURE_2D, mapTexture);

		map.Render(&mapShader);
		shader.Use();
		pCamera->Use(&shader);

		// Swap front and back buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.Delete();
	mapShader.Delete();
	glfwTerminate();

	delete pCamera;
	return 0;
}