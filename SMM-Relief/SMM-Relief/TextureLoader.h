#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stb_image.h>
#include <GL/glew.h>

unsigned int CreateTexture(const std::string& strTexturePath);

unsigned int CreateSkyboxTexture(const std::vector<std::string>& skyboxFaces);