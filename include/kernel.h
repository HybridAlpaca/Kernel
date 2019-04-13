#pragma once

#include <stdint.h>

struct registry_api;

struct api_desc_t
{
	uint16_t dep_count;
	const char * const * deps;

	void * api;

	void (*start)(registry_api * registry);
	void (*stop)(registry_api * registry);
	void (*update)(registry_api * registry);
};

struct registry_api
{
	void (*add_api)(const char * name, const api_desc_t * desc);

	void * (*get_api)(const char * name);

	void (*remove_api)(const char * name);

	void (*update_all)();

	void (*stop_all)();
};
