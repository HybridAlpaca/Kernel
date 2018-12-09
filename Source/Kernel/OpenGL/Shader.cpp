/*
#include <Kernel/Shader.h>
#include <Kernel/Graphics/Shader.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>

using namespace Kernel;

Shader::Shader(cchar vertexPath, cchar fragmentPath)
{
	// Compile individual shaders
	Graphics::Shader vertex{Graphics::ShaderType::VERTEX};
	Graphics::Shader fragment{Graphics::ShaderType::FRAGMENT};

	vertex.CompileFromPath(vertexPath);
	fragment.CompileFromPath(fragmentPath);

	// Link the program
	Graphics::Shader shaders [] {vertex, fragment};
	ID = Graphics::LinkShaders(shaders, 2);

	// Destroy individual shaders
	vertex.Destroy();
	fragment.Destroy();

	// Find uniform block
	uint32 UBIndex = glGetUniformBlockIndex(ID, "Matrices");
	glUniformBlockBinding(ID, UBIndex, 0);

	// Get block size
	GLint size;
	glGetActiveUniformBlockiv(ID, UBIndex, GL_UNIFORM_BLOCK_DATA_SIZE, & size);

	// Create uniform buffer object
	glGenBuffers(1, & UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Specify usable range within buffer
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, size);
}

void Shader::SetUniformData(char * data, uint32 size)
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Shader::Use()
{
	glUseProgram(ID);
}
*/
