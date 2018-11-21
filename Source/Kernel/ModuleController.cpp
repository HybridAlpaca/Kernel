#include <Kernel/ModuleController.h>

using namespace Kernel;

ModuleController::ModuleController()
{
	/// @note Dynamic alloc
	modules = new ModuleDesc[256];
}

ModuleController::~ModuleController()
{
	/// @note Dynamic free
	delete[] modules;
}

Handle ModuleController::RegisterModule(const ModuleRegisterProc * registerModule)
{
	registerModule(& modules[moduleCount++]);

	return moduleCount - 1;
}
