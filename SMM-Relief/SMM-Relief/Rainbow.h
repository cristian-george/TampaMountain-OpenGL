#pragma once
#include <string>
#include "Shader.h"
#include <vector>
#include "TextureLoader.h"
#include "Camera.h"

class Rainbow
{
public:
	Rainbow() = default;
	Rainbow(const std::string& resourcesFolder, Shader& rainbowShader);

	void Render(Camera* pCamera, Shader& rainbowShader);

private:
	void InitRainbow(Shader& rainbowShader);

private:
	unsigned int rainbowTexture;
	unsigned int rainbowVAO, rainbowVBO;

	std::string resourcesFolder;
};