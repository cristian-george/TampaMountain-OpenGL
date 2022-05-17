#include "InscriptionSign.h"

InscriptionSign::InscriptionSign(const std::string& resourcesFolder, Shader& signShader) : sign("Sign.obj")
{
	InitInscriptionSign(resourcesFolder, signShader);

	signShader.Use();
	signShader.SetVec3("lightColor", glm::vec3(0.6f, 0.6f, 0.6f));
}

void InscriptionSign::Render(Camera* camera, Shader& signShader)
{
	signShader.Use();
	camera->UpdateCameraVectors();
	camera->Use(signShader);
	glBindTexture(GL_TEXTURE_2D, signTexture);
	sign.Render(signShader);
}

void InscriptionSign::InitInscriptionSign(const std::string& resourcesFolder, Shader& signShader)
{
	signShader.Set("Texture.shader");

	signTexture = CreateTexture(resourcesFolder + "\\white.jpg");
	signShader.SetInt("sign", 2);

	sign.SetScale(glm::vec3(12.5f, 12.5f, 12.5f));
	sign.SetPosition(glm::vec3(-50.0f, 260.0f, 9.0f));
	sign.SetRotation(glm::vec3(0.0f, 215.0f, 0.0f));
	sign.InitVAO();
}