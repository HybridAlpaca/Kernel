/**
 * @file module_loader.cpp
 * @author StickyFingies
 *
 * @brief 64-bit Windows implementation of the kernel's module loader API
 *
 * @version 0.0.1
 * @date 2019-05-01
 *
 * @copyright Copyright (c) 2019
 */

#include "../module_loader.h"

#include <windows.h>

#include <vector>

/**
 * @struct module_loader_o
 *
 * @brief All global state needed to be kept by the POSIX module loaded implementation
 */
struct module_loader_o
{
	/// @brief List of handles to all loaded modules
	std::vector<HINSTANCE> dll_handles;
};

static module_loader_o state{};

const char * module_load_files(const char ** file_paths, uint16_t file_count)
{
	// Grow list to fit files (optimization)

	state.dll_handles.reserve(file_count);

	// Load all provided modules

	for (uint16_t i = 0; i < file_count; ++i)
	{
		// Load module into memory

		HINSTANCE handle = LoadLibraryA(file_paths[i]);

		// Capture loading errors

		if (!handle)
		{
			return "There was an error, but the Windows API makes it difficult to retrieve.  Ask Seth about fixing this.";
		}

		//
		// @todo Load module symbols here
		//

		// Add module to list

		state.dll_handles.push_back(handle);
	}

	// No errors :)

	return nullptr;
}

void module_unload_all()
{
	// Close every loaded module

	for (const auto & handle : state.dll_handles)
	{
		FreeLibrary(handle);
	}
}
