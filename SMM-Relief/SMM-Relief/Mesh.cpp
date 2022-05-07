#include "Mesh.h"
#include <ext/matrix_transform.hpp>
#include "OBJLoader.h"

void Mesh::InitVertexData(Vertex* vertexArray, const unsigned& numberOfVertices, GLuint* indexArray, const unsigned& numberOfIndices)
{
	for (size_t index = 0; index < numberOfVertices; ++index)
	{
		vertices.push_back(vertexArray[index]);
	}

	for (size_t index = 0; index < numberOfIndices; ++index)
	{
		indices.push_back(indexArray[index]);
	}
}

void Mesh::InitMaterials()
{
	for (size_t i = 0; i < materials.size(); ++i)
	{
		for (size_t j = 0; j < vertices.size(); ++j)
		{
			if (vertices[j].ColorID == i)
			{
				vertices[j].Ambient = materials[i].Ambient;
				vertices[j].Diffuse = materials[i].Diffuse;
				vertices[j].Specular = materials[i].Specular;
			}
		}
	}
}

void Mesh::UpdateModelMatrix()
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::scale(modelMatrix, scale);
}

Mesh::Mesh(const std::string& OBJfile)
{
	position = glm::vec3(0.f);
	rotation = glm::vec3(0.f);
	scale = glm::vec3(5.0f);

	const auto& files = LoadOBJ(OBJfile.c_str());
	vertices = files.first;
	materials = files.second;

	InitMaterials();
	UpdateModelMatrix();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(2, &EBO);
}

void Mesh::InitVAO()
{
	//Create VAO
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	//GEN EBO AND BIND AND SEND DATA
	if (indices.size() > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	}

	//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::Position));
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::Color));
	glEnableVertexAttribArray(1);
	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::TextureCoords));
	glEnableVertexAttribArray(2);
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::Normal));
	glEnableVertexAttribArray(3);
	//Ambient
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::Ambient));
	glEnableVertexAttribArray(4);
	//Diffuse
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::Diffuse));
	glEnableVertexAttribArray(5);
	//Specular
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Vertex::Specular));
	glEnableVertexAttribArray(6);

	//BIND VAO 0
	glBindVertexArray(0);
}

void Mesh::Render(Shader* shader)
{
	shader->Use();
	UpdateModelMatrix();

	shader->SetMat4("model", modelMatrix);
	glBindVertexArray(VAO);
	if (indices.empty())
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	else
	{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Mesh::SetPosition(const glm::vec3& position)
{
	this->position = position;
	UpdateModelMatrix();
}

void Mesh::SetRotation(const glm::vec3& rotation)
{
	this->rotation = rotation;
	UpdateModelMatrix();
}

void Mesh::SetModel(const glm::mat4& model)
{
	modelMatrix = model;
	UpdateModelMatrix();
}

void Mesh::SetScale(const glm::vec3& scale)
{
	this->scale = scale;
	UpdateModelMatrix();
}

void Mesh::SetColor(int index, const glm::vec3& rgb)
{
	int found = 0;
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		if (vertices[i].ColorID == index)
		{
			vertices[i].Color = rgb;
			found++;
		}
	}
	std::cout << "found: " << found << '\n';
}

const glm::mat4 Mesh::GetModel() const
{
	return modelMatrix;
}

const glm::vec3 Mesh::GetRotation() const
{
	return rotation;
}

const glm::vec3 Mesh::GetPosition() const
{
	return position;
}

const std::vector<Material> Mesh::GetMaterials() const
{
	return materials;
}