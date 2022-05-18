#pragma once
#define GLM_FORCE_CTOR_INIT
#include <glm.hpp>

class Vertex
{
public:
	Vertex() = default;

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextureCoords;

	glm::vec3 Color;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	int ColorID = -1;
};