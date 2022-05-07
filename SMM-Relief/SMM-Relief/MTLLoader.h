#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Material.h"

static std::vector<Material> LoadMTL(const char* file_name)
{
	int pos = -1;
	std::vector<Material> materials;
	std::stringstream ss;
	std::ifstream fin(file_name);
	if (!fin.is_open())
	{
		std::cout << "Failed to load " << file_name << '\n';
		return std::vector<Material>();
	}

	std::string line;
	std::string prefix;
	GLint value;
	while (std::getline(fin, line))
	{
		//get prefix
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "newmtl") //vertex position
		{
			materials.push_back(Material());
			pos++;
		}
		else
			if (prefix == "Ka") //texture coords
			{
				glm::vec3 ambient;
				ss >> ambient.x >> ambient.y >> ambient.z;
				materials[pos].Ambient = ambient;
			}
			else
				if (prefix == "Kd") //vertex normals
				{
					glm::vec3 diffuse;
					ss >> diffuse.x >> diffuse.y >> diffuse.z;
					materials[pos].Diffuse = diffuse;
				}
				else
					if (prefix == "Ks") //vertex normals
					{
						glm::vec3 specular;
						ss >> specular.x >> specular.y >> specular.z;
						materials[pos].Specular = specular;
					}
	}
	//debug
	std::cout << "MTL file " << file_name << " loaded successfully with " << pos + 1 << " materials!\n";
	return materials;
}