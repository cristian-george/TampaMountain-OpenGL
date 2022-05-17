#include "Application.h"

Camera* Application::camera = nullptr;
Shader Application::skyboxShader;
Shader Application::mapShader;
Shader Application::signShader;

void Application::Start()
{
	if (!InitWindow())
	{
		std::cout << "Couldn't initialize GLFW window !" << std::endl;
		return;
	}

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 400.0f, 300.0f));

	std::filesystem::path localPath = std::filesystem::current_path();
	std::string resourcesFolder = localPath.string() + "\\Resources";

	Skybox skybox(resourcesFolder, skyboxShader);
	Terrain terrain(resourcesFolder, mapShader);
	InscriptionSign inscriptionSign(resourcesFolder, signShader);

	Render(skybox, terrain, inscriptionSign);
}

Application::~Application()
{
	mapShader.Delete();
	skyboxShader.Delete();
	signShader.Delete();
	glfwTerminate();

	delete camera;
}

bool Application::InitWindow()
{
	if (!glfwInit())
		return false;

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Explorare relief real", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetKeyCallback(window, KeyCallback);

	glfwSetCursorPos(window, static_cast<double>(SCR_WIDTH) / 2, static_cast<double>(SCR_HEIGHT) / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void Application::Render(Skybox& skybox, Terrain& terrain, InscriptionSign& inscriptionSign)
{
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
		ProcessInput(window, camera, deltaTime);

		// Render here
		terrain.Render(camera, mapShader);
		skybox.Render(camera, skyboxShader);
		inscriptionSign.Render(camera, signShader);

		// Swap front and back buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	camera->Reshape(width, height);
}

void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	camera->MouseControl((float)xpos, (float)ypos);
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yOffset)
{
	camera->ProcessMouseScroll((float)yOffset);
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (tolower(key) == 'i' && lightIntensityValue.x < 0.9f)
	{
		lightIntensityValue += glm::vec3(0.05f);
	}

	if (tolower(key) == 'o' && lightIntensityValue.x > 0.2f)
	{
		lightIntensityValue -= glm::vec3(0.05f);
	}

	skyboxShader.Use();
	skyboxShader.SetVec3("lightColor", lightIntensityValue);
	mapShader.Use();
	mapShader.SetVec3("lightColor", lightIntensityValue);

	skyLight = lightIntensityValue.x;
}