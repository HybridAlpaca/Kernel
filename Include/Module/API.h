#pragma once

#include <Subframe/Subframe.h>

struct ModuleDesc
{
	cchar name;

	struct {
		uint32 major;
		uint32 minor;
		uint32 patch;
	} version;
};

struct SystemServices
{
	struct {
		uint32 major;
		uint32 minor;
		uint32 patch;
	} version;

	void (* RegisterModule)(const ModuleDesc *);
};

using LoadProc = void(const SystemServices *);
using UnloadProc = void();
