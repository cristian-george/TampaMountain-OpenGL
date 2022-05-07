#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderSource.h"
#include <glm.hpp>

class Shader
{
	enum class ShaderType
	{
		NONE = -1,
		VERTEX,
		FRAGMENT
	};

public:
	ShaderSource Source;
	unsigned int ShaderIndex;

	void Set(const std::string& sourceFilePath);
	void Use();
	void Delete();
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, const float& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;

protected:
	ShaderSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};