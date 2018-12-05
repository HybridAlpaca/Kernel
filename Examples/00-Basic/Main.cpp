/**
 * @file Main.cpp
 * @author Seth Traman
 *
 * @brief Entry point for 00-HelloWorld example program
 **/

#include <Subframe/Subframe.h>
#include <Kernel/Window.h>
#include <Kernel/Context.h>
#include <Kernel/Renderer.h>

/*
 * Requirements
 * x Display window
 * - Load model
 * - Render model
 *
 * Window Requirements
 * x Open
 * x Close
 * o Input (@todo input callbacks)
 * x Swap buffers
 *
 * Model Requirements
 * - OBJ compatable
 *
 * Render Requirements
 * - Textures
 * - Lighting
 * - Camera
 *
 */

static constexpr const uint32 WIDTH  = 640;
static constexpr const uint32 HEIGHT = 480;

int32 main(int32 argc, char ** argv)
{
	Kernel::WindowPlatformData data = Kernel::CreateWindow("Hello, World!", WIDTH, HEIGHT);

	Kernel::GraphicsContext ctx{data};
	ctx.Create(WIDTH, HEIGHT);

	auto tri = Kernel::Renderer::CreateTriangle();

	while (Kernel::WindowOpen())
	{
		Kernel::ProcessWindowEvents();

		Kernel::Renderer::ClearScreen();
		Kernel::Renderer::DrawTriangle(tri);

		ctx.SwapBuffers();
	}

	Kernel::Renderer::DestroyTriangle(tri);

	ctx.Destroy();
	Kernel::DestroyWindow();

	return 0;
}
