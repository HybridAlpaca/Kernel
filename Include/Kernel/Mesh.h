#pragma once

#include <Subframe/Subframe.h>

#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture
{
	enum class Type
	{
		DIFFUSE,
		SPECULAR,
		NORMAL,
		HEIGHT
	};

	uint32 id;
	Type type;

	std::string path;
};

struct Mesh
{
	std::vector<Texture> textures;

	uint32 VAO;
	uint32 VBO;
	uint32 EBO;
	uint32 indexCount;
};
