/**
 * @file Main.cpp
 * @author Seth Traman
 *
 * @brief Entry point for 00-HelloWorld example program
 **/

#include <Subframe/Subframe.h>
#include <Kernel/Window.h>
#include <Kernel/Context.h>
#include <Kernel/Debug.h>

#include <Kernel/Camera.hpp>
#include <Kernel/Graphics/Shader.h>
#include <Kernel/Graphics/Program.h>
#include <Kernel/Graphics/Uniform.h>
#include <Kernel/Model.h>

#include <GL/glew.h>

using namespace Kernel;

static constexpr const uint32 WIDTH  = 1024;
static constexpr const uint32 HEIGHT = 768;

static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

struct UniformData
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

int32 main(int32 argc, char ** argv)
{
	// Set up our global debugging context
	Kernel::DebugInit();

	// Create a window and a rendering context
	Kernel::WindowPlatformData data = Kernel::CreateWindow("Hello, World!", WIDTH, HEIGHT);
	Kernel::GraphicsContext ctx{data};
	ctx.Create(WIDTH, HEIGHT);

	// Initialize & compile shaders
	Graphics::Shader shaders []
	{
		Graphics::Shader{Graphics::ShaderType::VERTEX},
		Graphics::Shader{Graphics::ShaderType::FRAGMENT}
	};
	shaders[0].CompileFromPath("/home/cellman123/Desktop/Kernel/Assets/ModelLoading.vs");
	shaders[1].CompileFromPath("/home/cellman123/Desktop/Kernel/Assets/ModelLoading.fs");

	// Link program
	Graphics::Program program;
	program.Link(shaders, 2);

	// Destroy shaders
	shaders[0].Destroy();
	shaders[1].Destroy();

	// Load in a model
	Model ourModel{"/home/cellman123/Desktop/Kernel/Assets/nanosuit/nanosuit.obj"};

	// Apply some transforms
	ourModel.transform = glm::translate(ourModel.transform, glm::vec3(0.0f, -0.75f, -1.0f));
	ourModel.transform = glm::scale(ourModel.transform, glm::vec3(0.1f, 0.1f, 0.1f));

	// Create a uniform block
	Graphics::Uniform uBlock;
	uBlock.Create(sizeof(UniformData));

	// Bind block to program
	program.BindUniformSlot(uBlock.Slot(), "Matrices");

	// Create some data
	UniformData uniform;
	uniform.projection = glm::perspective(glm::radians(camera.Zoom), 640.0f / 680.0f, 0.1f, 100.0f);
	uniform.view       = camera.GetViewMatrix();
	uniform.model      = ourModel.transform;

	// Fill our block with data
	uBlock.Write((char *) & uniform, 0, sizeof(UniformData));

	// Bind our uniform block
	uBlock.Bind();

	// Main game loop
	while (Kernel::WindowOpen())
	{
		Kernel::DebugCPUBegin("Frame");

		// Process incoming user events
		Kernel::ProcessWindowEvents();

		// Clear the screen
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the shader we loaded from before
		program.Use();

		// Draw our model
		ourModel.Draw(program.Handle());

		// Swap front and back buffers
		// NOTE: This causes a full context flush.  This can be... slow...
		ctx.SwapBuffers();

		Kernel::DebugCPUEnd();
	}

	// Clean up
	program.Destroy();
	ctx.Destroy();
	Kernel::DestroyWindow();
	Kernel::DebugShutdown();

	return 0;
}
