#include <Subframe/Subframe.h>
#include <Kernel/SharedLib.h>
#include <Module/API.h>

#include <iostream>

void Register(const ModuleDesc * desc)
{
	std::cout << "[Kernel] :: Load :: " << desc -> name
		<< " v." << desc -> version.major
		<< "." << desc -> version.minor
		<< "." << desc -> version.patch
		<< '\n';
}

int32 main(int32 argc, char ** argv)
{
	Kernel::SharedLib lib("/home/cellman123/Desktop/Kernel/Binaries/Examples/mHelloModule.so");

	LoadProc * Load = (LoadProc *) lib.ProcAddress("Load");
	UnloadProc * Unload = (UnloadProc *) lib.ProcAddress("Unload");

	SystemServices services;
	services.version.major = 0;
	services.version.minor = 0;
	services.version.patch = 1;
	services.RegisterModule = Register;

	Load(& services);
	Unload();

	return 0;
}
