#pragma once
#include <string>
#include "Shader.h"
#include <vector>
#include "TextureLoader.h"
#include "Camera.h"

class Skybox
{
public:
	Skybox() = default;
	Skybox(const std::string& resourcesFolder, Shader& skyboxShader);

	void Render(Camera* pCamera, Shader& skyboxShader);

private:
	void SetSkyboxFaces();
	void InitSkybox(Shader& skyboxShader);

private:
	unsigned int skyboxTexture;
	unsigned int skyboxVAO, skyboxVBO;
	std::vector<std::string> skyboxFaces;

	std::string resourcesFolder;
};