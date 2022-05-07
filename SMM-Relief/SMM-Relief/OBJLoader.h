#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Material.h"
#include "Vertex.h"
#include "MTLLoader.h"

static std::pair<std::vector<Vertex>, std::vector<Material>> LoadOBJ(const char* fileName)
{
	//vertex vectors
	std::vector<glm::fvec3> vertexPosition;
	std::vector<glm::fvec3> vertexNormal;
	std::vector<glm::fvec2> vertexTextureCoord;

	//face vectors
	std::vector<GLint> vertexPosIndices;
	std::vector<GLint> vertexTexCoordIndices;
	std::vector<GLint> vertexNormalIndices;
	std::vector<GLint> colorIndices;

	//Vertex array
	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream fin(fileName);
	if (!fin.is_open())
	{
		std::cout << "Failed to load " << fileName << '\n';
		return std::make_pair(std::vector<Vertex>(), std::vector <Material>());
	}

	std::string line;
	std::string materialName;
	std::string prefix;
	glm::vec3 temp_vec3{};
	glm::vec2 temp_vec2{};
	GLint temp_glint;
	GLint matNumber = -1;

	while (std::getline(fin, line))
	{
		//get prefix
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "mtllib") //material
		{
			ss >> materialName;
		}
		else
			if (prefix == "v") //vertex position
			{
				ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
				vertexPosition.push_back(temp_vec3);
			}
			else
				if (prefix == "vt") //texture coords
				{
					ss >> temp_vec2.x >> temp_vec2.y;
					vertexTextureCoord.push_back(temp_vec2);
				}
				else
					if (prefix == "vn") //vertex normals
					{
						ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
						vertexNormal.push_back(temp_vec3);
					}
					else
						if (prefix == "f")
						{
							int counter = 0;
							while (ss >> temp_glint)
							{
								//indices into correct arrays
								if (counter == 0)
								{
									vertexPosIndices.push_back(temp_glint);
									colorIndices.push_back(matNumber);
								}
								else if (counter == 1)
									vertexTexCoordIndices.push_back(temp_glint);
								else if (counter == 2)
									vertexNormalIndices.push_back(temp_glint);

								//characters
								if (ss.peek() == '/')
								{
									counter++;
									ss.ignore(1, '/');
								}
								else if (ss.peek() == ' ')
								{
									counter++;
									ss.ignore(1, ' ');
								}

								//reset the counter
								if (counter > 2)
								{
									counter = 0;
								}
							}
						}
						else if (prefix == "usemtl")
						{
							matNumber++;
						}

	}

	//Load in all indices
	vertices.resize(vertexPosIndices.size(), Vertex());
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].Position = vertexPosition[vertexPosIndices[i] - 1];
		vertices[i].TextureCoords = vertexTextureCoord[vertexTexCoordIndices[i] - 1];
		vertices[i].Normal = -0.5f * vertexNormal[vertexNormalIndices[i] - 1];
		vertices[i].ColorID = colorIndices[i];
		vertices[i].Color = glm::vec3(1.0f, 0.0f, 1.0f);
	}

	//debug
	std::cout << "OBJ file " << fileName << " loaded successfully with " << vertices.size() << " vertices" << "!\n";
	std::vector <Material> materials;
	std::ifstream mat(materialName);
	mat.close();
	materials = LoadMTL(materialName.c_str());

	return std::make_pair(vertices, materials);
}