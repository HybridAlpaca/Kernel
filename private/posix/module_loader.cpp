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

static const char * module_load_files(module_loader_i * api, const char * const * file_paths, uint16_t file_count)
{
	auto & inst = api->inst;

	// Grow list to fit files (optimization)

	inst->dll_handles.reserve(file_count);

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

		inst->dll_handles.push_back(handle);
	}

	// No errors :)

	return nullptr;
}

static void module_unload_all(module_loader_i * api)
{
	auto & inst = api->inst;

	// Close every loaded module

	for (const auto & handle : inst->dll_handles)
	{
		dlclose(handle);
	}
}

module_loader_i * create_default_module_loader()
{
	auto inst = new module_loader_o{};

	auto module_loader = new module_loader_i
	{
		inst,
		module_load_files,
		module_unload_all
	};

	return module_loader;
}

void destroy_default_module_loader(module_loader_i * module_loader)
{
	delete module_loader->inst;
	delete module_loader;
}
