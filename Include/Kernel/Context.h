#pragma once

/**
 * @file Context.h
 * @brief Platform-independant methods for OpenGL context creation
 *
 * @todo Merge Window and Context
 **/

#include <Subframe/Subframe.h>
#include "Window.h"

namespace Kernel
{
	/**
	 * @class GraphicsContext
	 * @author Seth Traman
	 *
	 * @brief Captures an OpenGL context
	 *
	 * @todo [platform] Windows
	 * @note Requires a window.  Window *must* outlive the context
	 **/
	class GraphicsContext final
	{
		/// PImpl
		struct Driver;
		Driver * driver;

		/// Native window handles etc. required for context usage
		WindowPlatformData platform;

	public:

		explicit GraphicsContext(const WindowPlatformData & platformData);

		GraphicsContext(const GraphicsContext &) = delete;
		GraphicsContext(GraphicsContext &&)      = delete;

		~GraphicsContext();

		GraphicsContext & operator=(const GraphicsContext &) = delete;
		GraphicsContext & operator=(GraphicsContext &&)      = delete;

		/// Create a context in the graphics backend
		/// @param [in] width		Width of the window this context was created with
		/// @param [in] height	Height of the window this context was created with
		void Create(uint32 width, uint32 height);

		/// Desetroy this context
		void Destroy();

		/// Swap window buffers
		void SwapBuffers();

		/// Is this context valid / usable
		/// @return true if the context is valid, false otherwise
		bool Valid() const;
	};
}
