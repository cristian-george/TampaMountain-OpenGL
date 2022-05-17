#pragma once
#include <glm.hpp>
#include <GL/glew.h>

class Material
{
public:
	Material() = default;

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	GLint diffuseTex;
	GLint specularTex;
};