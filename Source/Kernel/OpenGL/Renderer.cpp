#include <Kernel/Renderer.h>

#include <GL/glew.h>

#include <iostream>

using namespace Kernel;

namespace
{
	float rawVertices []
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	cchar rawVertShader =
		"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

	cchar rawFragShader =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
}

Renderer::Triangle Renderer::CreateTriangle()
{
	// vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, & rawVertShader, nullptr);
	glCompileShader(vertexShader);

	// fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, & rawFragShader, nullptr);
	glCompileShader(fragmentShader);

	// link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// destroy shaders
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint VAO;
	GLuint VBO;

	glGenVertexArrays(1, & VAO);
	glGenBuffers(1, & VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rawVertices), rawVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return Renderer::Triangle {VAO, VBO, shaderProgram};
}

void Renderer::DestroyTriangle(Triangle & tri)
{
	glDeleteVertexArrays(1, & tri.vao);
	glDeleteBuffers(1, & tri.vbo);
	glDeleteProgram(tri.program);

	tri.vao = 0;
	tri.vbo = 0;
	tri.program = 0;
}

void Renderer::DrawTriangle(const Renderer::Triangle & tri)
{
	glUseProgram(tri.program);
	glBindVertexArray(tri.vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::ClearScreen()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
