#include "Terrain.h"

Terrain::Terrain(const std::string& resourcesFolder, Shader& mapShader) : map("Map.obj")
{
	InitMap(resourcesFolder, mapShader);

	mapShader.Use();
	mapShader.SetVec3("lightColor", glm::vec3(0.6f, 0.6f, 0.6f));
}

void Terrain::Render(Camera* pCamera, Shader& mapShader)
{
	mapShader.Use();
	pCamera->UpdateCameraVectors();
	pCamera->Use(mapShader);

	glBindTexture(GL_TEXTURE_2D, mapTexture);
	map.Render(mapShader);
}

void Terrain::InitMap(const std::string& resourcesFolder, Shader& mapShader)
{
	mapShader.Set("Texture.shader");

	mapTexture = CreateTexture(resourcesFolder + "\\MapTexture\\GOOGLE_SAT_WM.jpg");
	mapShader.SetInt("map", 1);

	map.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	map.SetPosition(glm::vec3(0.0f));
	map.InitVAO();
}
