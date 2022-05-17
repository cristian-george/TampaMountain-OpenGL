#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "TextureLoader.h"

class InscriptionSign
{
public:
	InscriptionSign() = default;
	InscriptionSign(const std::string& resourcesFolder, Shader& signShader);

	void Render(Camera* camera, Shader& signShader);

private:
	void InitInscriptionSign(const std::string& resourcesFolder, Shader& signShader);

private:
	Mesh sign;
	unsigned int signTexture;
};

