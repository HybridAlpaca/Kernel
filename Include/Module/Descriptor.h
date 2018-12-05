#pragma once

#include <Subframe/Subframe.h>

struct ModuleVersion
{
	uint16 major;
	uint16 minor;
	uint16 patch;
};

struct ModuleDesc
{
	cchar name;
	cchar author;
	ModuleVersion version;
};
