#include "../state.h"

#include <dlfcn.h>

#include <filesystem>
#include <iostream>

struct module_loader_o
{
	std::vector<void *> handles;
	std::vector<module_desc_t *> modules;
};

static module_loader_o state;

void load_modules_dir(const char * path)
{
	for (const auto & entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().extension() != ".mod")
		{ continue; }

		void * handle = dlopen(entry.path().c_str(), RTLD_LAZY);

		if (handle == nullptr)
		{
			std::cout << dlerror() << std::endl;
		}

		auto module = static_cast<module_desc_t *>(dlsym(handle, "module"));

		state.handles.push_back(handle);
		state.modules.push_back(module);
	}
}

void unload_modules()
{
	for (const auto & entry : state.handles)
	{
		dlclose(entry);
	}
}

uint16_t get_loaded_modules(module_desc_t *** modules)
{
	(*modules) = state.modules.data();

	return state.modules.size();
}
