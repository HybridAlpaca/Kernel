#include <Kernel/Graphics/Shader.h>
#include <Kernel/Debug.h>

#include <GL/glew.h>

#include <fstream>
#include <sstream>

using namespace Kernel::Graphics;

static void CheckCompilation(GLuint shader);

Shader::Shader(ShaderType type)
: type{type}
{
	// Empty initializer
}

void Shader::CompileFromPath(cchar path)
{
	// Variables we'll need
	std::string code;
	std::ifstream file;

	// Open the file
	file.open(path);

	// Attempt to read contents
	if (file.is_open())
	{
		// Dump file contents into the stream
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();

		// Move stream into code string
		code = stream.str();
	}
	else
	{
		// File couldn't be opened for reading.  Big oof.
		Kernel::DebugLog("[Error] Shader file could NOT be opened for reading:");
		Kernel::DebugLog(path);
		return;
	}

	// Figure out shader type
	GLenum shaderType = (type == ShaderType::VERTEX) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;

	// Create the shader
	handle = glCreateShader(shaderType);

	// Provide shader with source code
	const char * cCode = code.c_str();
	glShaderSource(handle, 1, & cCode, nullptr);

	// Compile & check for errors
	glCompileShader(handle);
	CheckCompilation(handle);
}

void Shader::Destroy()
{
	// Self-explanatory
	glDeleteShader(handle);
}

void CheckCompilation(uint32 shader)
{
	// Variables we'll need
	GLint success;
	GLchar infoLog[1024];

	// Get compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, & success);
	if (success == false)
	{
		// Retrieve log and send to debugger
		glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
		Kernel::DebugLog("[Error] Shader compilation error:");
		Kernel::DebugLog(infoLog);
	}
}
