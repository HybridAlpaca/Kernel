#include <kernel.h>
#include "state.h"

#include <filesystem>
#include <iostream>
#include <string>

static bool running{true};

static std::string root_dir{};

kernel_api kernel
{
	api_impl_add,
	api_impl_first,
	api_impl_next,
	[](){ running = false; },
	[](){ return root_dir.c_str(); }
};

int main(int argc, char ** argv)
{
	const std::string exe_path{std::filesystem::weakly_canonical(argv[0])};

	unsigned int slash_pos = exe_path.rfind('/');

	// Get absolute path to lib dir.  This assumes a 64 bit system.
	// @todo Get this information from CMake?

	root_dir = exe_path.substr(0, slash_pos + 1) + "../";

	const auto lib_dir = root_dir + "lib64";

	load_modules_dir(lib_dir.c_str());

	module_t ** modules;

	uint16_t module_count = get_loaded_modules(&modules);

	for (uint16_t i{0}; i < module_count; ++i)
	{
		registry_add_module(modules[i]);
	}

	registry_start_modules();

	while (running)
	{
		registry_execute_modules();
	}

	registry_stop_modules();

	unload_modules();

	return 0;
}
