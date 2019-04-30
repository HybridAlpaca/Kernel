#include "../module_loader.h"

#include <dlfcn.h>

#include <vector>

/**
 * @struct module_loader_o
 *
 * @brief All global state needed to be kept by the POSIX module loaded implementation
 */
struct module_loader_o
{
	/// @brief List of handles to all loaded modules
	std::vector<void *> dll_handles;
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

		state.dll_handles.push_back(handle);
	}
}

void module_unload_all()
{
	// Close every loaded module

	for (const auto & handle : state.dll_handles)
	{
		dlclose(handle);
	}
}
