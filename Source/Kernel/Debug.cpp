#include <Kernel/Debug.h>

#include <External/Remotery/Remotery.h>

namespace
{
	Remotery * globalInstance;
	bool valid = true;
}

void Kernel::DebugInit()
{
	rmtError error = rmt_CreateGlobalInstance(& globalInstance);

	if (error != RMT_ERROR_NONE)
	{
		valid = false;
	}
}

void Kernel::DebugShutdown()
{
	if (valid == false) return;

	rmt_DestroyGlobalInstance(globalInstance);
	valid = false;
}

void Kernel::DebugCPUBegin(cchar id)
{
	if (valid == false) return;

	_rmt_BeginCPUSample(id, 0, nullptr);
}

void Kernel::DebugCPUEnd()
{
	if (valid == false) return;

	rmt_EndCPUSample();
}

void Kernel::DebugLog(cchar msg)
{
	rmt_LogText(msg);
}
