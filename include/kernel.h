#pragma once

#include <stdint.h>

struct kernel_api;

struct module_t
{
	const char * id;

	const char * const * exports;
	const char * const * imports;

	uint16_t export_count;
	uint16_t import_count;

	bool (*start)(kernel_api * kernel);
	bool (*stop)();
	bool (*update)();
};

struct kernel_api
{
	void (*api_impl_add)(const char * name, void * impl);

	void * (*api_impl_first)(const char * name);

	void * (*api_impl_next)(const char * name, void * prev);

	void (*shutdown)();

	const char * (*get_root_dir)();
};
