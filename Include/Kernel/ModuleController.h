#pragma once

#include <Subframe/Subframe.h>
#include <Module/API.h>

namespace Kernel
{
	using Handle = uint16;

	class ModuleController final
	{
		ModuleDesc * modules;
		uint32 moduleCount;

	public:

		ModuleController();
		ModuleController(const ModuleController &) = delete;
		ModuleController(ModuleController &&) = delete;
		~ModuleController();

		ModuleController & operator=(const ModuleController &) = delete;
		ModuleController & operator=(ModuleController &&) = delete;

		Handle RegisterModule(const ModuleRegisterProc * registerModule);

		inline const ModuleDesc & GetModuleDesc(Handle handle) const { return modules[handle]; }
	};
}
