#pragma once

#include <Subframe/Subframe.h>
#include <Kernel/Debug.h>

#include <string>

enum class ShaderType
{
	PROGRAM,

	VERTEX,
	FRAGMENT
};

class Shader
{
	uint32 UBO;

public:

	uint32 ID;

	Shader (cchar vertexPath, cchar fragmentPath);

	void SetUniformData(char * data, uint32 size);

	void Use();
};
