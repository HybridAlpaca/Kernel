/**
 * @file module_loader.cpp
 * @author StickyFingies
 *
 * @brief POSIX implementation of the kernel's module loading interface
 *
 * @version 0.0.1
 * @date 2019-05-01
 *
 * @copyright Copyright (c) 2019
 */

#include "../module_loader.h"

#include <dlfcn.h>

#include <vector>

/**
 * @struct module_loader_o
 *
 * @brief All internal state needed to be kept by the POSIX module loaded implementation
 */
struct module_loader_o
{
	/// @brief List of handles to all loaded modules
	std::vector<void *> dll_handles;
};

static module_loader_o internal{};

const char * module_load_files(const char ** file_paths, uint16_t file_count)
{
	// Grow list to fit files (optimization)

	internal.dll_handles.reserve(file_count);

	// Load all provided modules

	for (uint16_t i = 0; i < file_count; ++i)
	{
		// Load module into memory

		void * handle = dlopen(file_paths[i], RTLD_LAZY);

		// Capture loading errors

		if (!handle)
		{
			return dlerror();
		}

		//
		// @todo Load module symbols here
		//

		// Add module to list

		internal.dll_handles.push_back(handle);
	}

	// No errors :)

	return nullptr;
}

void module_unload_all()
{
	// Close every loaded module

	for (const auto & handle : internal.dll_handles)
	{
		dlclose(handle);
	}
}
