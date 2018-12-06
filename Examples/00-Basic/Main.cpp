/**
 * @file Main.cpp
 * @author Seth Traman
 *
 * @brief Entry point for 00-HelloWorld example program
 **/

#include <Subframe/Subframe.h>
#include <Kernel/Window.h>
#include <Kernel/Context.h>

#include <Kernel/Camera.hpp>
#include <Kernel/Model.h>

static constexpr const uint32 WIDTH  = 640;
static constexpr const uint32 HEIGHT = 480;

static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int32 main(int32 argc, char ** argv)
{
	Kernel::WindowPlatformData data = Kernel::CreateWindow("Hello, World!", WIDTH, HEIGHT);

	Kernel::GraphicsContext ctx{data};
	ctx.Create(WIDTH, HEIGHT);

	Shader ourShader("/home/cellman123/Desktop/Kernel/Assets/ModelLoading.vs", "/home/cellman123/Desktop/Kernel/Assets/ModelLoading.fs");

	Model ourModel("/home/cellman123/Desktop/Kernel/Assets/nanosuit/nanosuit.obj");

	// translate it down so it's at the center of the scene
	ourModel.transform = glm::translate(ourModel.transform, glm::vec3(0.0f, -1.75f, 0.0f));
	// it's a bit too big for our scene, so scale it down
	ourModel.transform = glm::scale(ourModel.transform, glm::vec3(0.2f, 0.2f, 0.2f));

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 640.0f / 680.0f, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	while (Kernel::WindowOpen())
	{
		Kernel::ProcessWindowEvents();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		ourModel.Draw(ourShader);

		ctx.SwapBuffers();
	}

	ctx.Destroy();
	Kernel::DestroyWindow();

	return 0;
}
