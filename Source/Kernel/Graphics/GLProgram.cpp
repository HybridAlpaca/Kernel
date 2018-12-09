#include <Kernel/Graphics/Program.h>
#include <Kernel/Graphics/Shader.h>

#include <GL/glew.h>

using namespace Kernel::Graphics;

Program::Program()
: handle{0}
{
	// Empty initializer
}

void Program::Link(const Shader * shaders, uint32 shaderCount)
{
	// Create program
	handle = glCreateProgram();

	// Attach shaders to program
	for (uint32 i = 0; i < shaderCount; ++i)
	{
		glAttachShader(handle, shaders[i].Handle());
	}

	// Link program
	glLinkProgram(handle);

	// Detach shaders from program
	for (uint32 i = 0; i < shaderCount; ++i)
	{
		glDetachShader(handle, shaders[i].Handle());
	}

	// Shaders can now be destroyed, unless they're going
	// to be used in other shader programs
}

void Program::Destroy()
{
	// Self-explanatory
	glDeleteProgram(handle);
	handle = 0;
}

void Program::BindUniformSlot(uint32 slot, cchar name)
{
	// Retrieve index into program where uniform block resides
	uint32 index = glGetUniformBlockIndex(handle, name);

	// Bind index into global slot
	glUniformBlockBinding(handle, index, slot);
}

void Program::Use() const
{
	// Self-explanatory
	glUseProgram(handle);
}
