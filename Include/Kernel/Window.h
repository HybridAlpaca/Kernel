#pragma once

/**
 * @file Window.h
 * @brief Functions and data structures relating to window management
 *
 * @todo Merge Window and Context
 **/

#include <Subframe/Subframe.h>

namespace Kernel
{
	/**
	 * @struct WindowPlatformData
	 * @brief Provides native OS window handles & information
	 **/
	struct WindowPlatformData
	{
		void * nativeDisplay; ///< Native handle to the window's display
		void * nativeWindow;  ///< Native window handle
		void * context;       ///< Handle to graphics context, if any
	};

	/// Create a desktop window
	/// @return Native window data for use in other systems
	/// @note Only one window may exist at once
	/// @todo Windows support
	/// @warning Not thread safe
	WindowPlatformData CreateWindow(cchar title, uint32 width, uint32 height);

	/// Destroy desktop window
	/// @warning Not thread safe
	void DestroyWindow();

	/// Queries the running state of the window
	/// @return Is the window open or not
	/// @todo Windows support
	/// @warning Not thread safe
	bool WindowOpen();

	/// Collects and acts on events from the OS
	/// @todo Windows support
	/// @warning Not thread safe
	void ProcessWindowEvents();
}
