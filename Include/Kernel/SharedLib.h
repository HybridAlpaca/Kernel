#pragma once

#include <Subframe/Subframe.h>

namespace Kernel
{
	class SharedLib
	{
		struct Driver;
		Driver * driver;

	public:

		SharedLib(cchar path);
		~SharedLib();

		void * ProcAddress(cchar symbol);
	};
}
