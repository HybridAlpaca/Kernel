#pragma once

#include <stdint.h>

static const char * LOAF_SERVICE_LOCATOR_ID = "loaf_service_locator_i";

static const uint16_t LOAF_SERVICE_LOCATOR_VERSION_MAJOR = 1;
static const uint16_t LOAF_SERVICE_LOCATOR_VERSION_MINOR = 0;

struct loaf_service_locator_i
{
	void * state;

	void (*construct)(loaf_service_locator_i * api);

	void (*destruct)(loaf_service_locator_i * api);

	void (*provide_service)(loaf_service_locator_i * api, const char * service_id, void * service);

	void * (*first_service)(loaf_service_locator_i * api, const char * service_id);

	void * (*next_service)(loaf_service_locator_i * api, void * previous);
};

extern "C" void loaf_load_service_locator(loaf_service_locator_i * locator);

extern "C" void loaf_unload_service_locator(loaf_service_locator_i * locator);
