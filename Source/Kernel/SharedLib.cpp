#include <Kernel/SharedLib.h>

#include <dlfcn.h>
#include <iostream>

using namespace Kernel;

struct SharedLib::Driver
{
	void * handle;
};

SharedLib::SharedLib(cchar path)
{
	driver = new Driver{};
	driver -> handle = dlopen(path, RTLD_LAZY);

	if (!driver -> handle)
	{
		std::cout << "Error loading shared library!" << '\n';
	}
}

SharedLib::~SharedLib()
{
	dlclose(driver -> handle);
	delete driver;
}

void * SharedLib::ProcAddress(cchar symbol)
{
	void * sym = dlsym(driver -> handle, symbol);

	if (!sym)
	{
		std::cout << "Error loading symbol!" << '\n';
	}

	return sym;
}
