#pragma once

namespace Kernel
{
	struct Shell
	{
		struct {
			uint32 major;
			uint32 minor;
			uint32 patch;
		} version;
	};
}
