#pragma once

#include <Subframe/Subframe.h>

struct ModuleDesc
{
	cchar name;
	cchar author;

	struct {
		uint32 major;
		uint32 minor;
		uint32 patch;
	} version;

	/// @todo Dependancies
	/// @todo Extensions
};

using ModuleRegisterProc = void (ModuleDesc *);
