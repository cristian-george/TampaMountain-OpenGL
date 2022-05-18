#include "Rainbow.h"

Rainbow::Rainbow(const std::string& resourcesFolder, Shader& rainbowShader)
{
	this->resourcesFolder = resourcesFolder;

	InitRainbow(rainbowShader);
}

void Rainbow::Render(Camera* pCamera, Shader& rainbowShader)
{
	glm::mat4 viewMatrix = pCamera->GetViewMatrix();
	glm::mat4 projMatrix = pCamera->GetProjectionMatrix();

	rainbowShader.Use();
	rainbowShader.SetMat4("view", viewMatrix);
	rainbowShader.SetMat4("projection", projMatrix);

	glBindVertexArray(rainbowVAO);
	glBindTexture(GL_TEXTURE_2D, rainbowTexture);

	glm::mat4 modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-700.0f, 300.0f, 300.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1500.0f, 1000.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, (float)glm::radians(180.0f), glm::vec3(1.f, 0.f, 0.f));
	//modelMatrix = glm::rotate(modelMatrix, (float)glm::radians(30.0f), glm::vec3(0.f, 1.f, 0.f));

	rainbowShader.SetMat4("model", modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Rainbow::InitRainbow(Shader& rainbowShader)
{
	float rainbowVertices[] =
	{
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &rainbowVAO);
	glGenBuffers(1, &rainbowVBO);
	glBindVertexArray(rainbowVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rainbowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rainbowVertices), &rainbowVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	rainbowTexture = CreateTexture(resourcesFolder + "\\rainbow.png");

	rainbowShader.Set("Blending.shader");
	rainbowShader.SetInt("rainbowTexture", 0);
}