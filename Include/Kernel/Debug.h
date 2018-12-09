#pragma once

#include <Subframe/Subframe.h>

namespace Kernel
{
	void DebugInit();

	void DebugShutdown();

	void DebugCPUBegin(cchar id);

	void DebugCPUEnd();

	void DebugLog(cchar msg);
}
