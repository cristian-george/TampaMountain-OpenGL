#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "TextureLoader.h"

class Terrain
{
public:
	Terrain() = default;
	Terrain(const std::string& resourcesFolder, Shader& mapShader);

	void Render(Camera* pCamera, Shader& mapShader);

private:
	void InitMap(const std::string& resourcesFolder, Shader& mapShader);

private:
	Mesh map;
	unsigned int mapTexture;
};