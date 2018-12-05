#pragma once

#include <Subframe/Subframe.h>

/**
 * @namespace Kernel::Renderer
 * @brief Toy graphics functions for drawing a triangle
 **/
namespace Kernel::Renderer
{
	struct Triangle
	{
		uint32 vao;     ///< Vertex array object
		uint32 vbo;     ///< Vertex buffer object
		uint32 program; ///< Shader program
	};

	Triangle CreateTriangle();
	void DestroyTriangle(Triangle & tri);

	void DrawTriangle(const Triangle & tri);

	void ClearScreen();
}
