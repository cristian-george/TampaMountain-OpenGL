#pragma once
#include <string>

class ShaderSource
{
public:
	ShaderSource() = default;

	std::string VertexSource;
	std::string FragmentSource;
};