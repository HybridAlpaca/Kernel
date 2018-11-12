#pragma once

#include <Subframe/Subframe.h>

namespace Kernel
{
	class ModuleController final
	{
		//

	public:

		ModuleController();
		~ModuleController();

		ModuleDesc * LoadModule(cchar path);
	};
}
