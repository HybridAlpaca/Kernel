#include <iostream>

#include <Module/API.h>

extern "C" void Load(const SystemServices * services)
{
	std::cout << "[Module] :: Load :: Kernel v." << services -> version.major
		<< "." << services -> version.minor
		<< "." << services -> version.patch << '\n';

	ModuleDesc desc;
	desc.name = "01 Hello Module";
	desc.version.major = 0;
	desc.version.minor = 0;
	desc.version.patch = 1;

	services -> RegisterModule(& desc);
}

extern "C" void Unload()
{
	std::cout << "Goodbye!" << '\n';
}
