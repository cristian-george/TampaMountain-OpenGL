#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include "Vertex.h"
#include "Material.h"
#include "Shader.h"

class Mesh
{
public:
	Mesh(const std::string& OBJfile);
	~Mesh();

	void InitVAO();
	void Render(Shader& shader);

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);

	void SetModel(const glm::mat4& model);
	void SetScale(const glm::vec3& scale);
	void SetColor(int index, const glm::vec3& rgb);

	const glm::mat4 GetModel() const;
	const glm::vec3 GetRotation() const;
	const glm::vec3 GetPosition() const;
	const std::vector<Material> GetMaterials() const;

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Material> materials;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void InitVertexData(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
	void InitMaterials();
	void UpdateModelMatrix();
};