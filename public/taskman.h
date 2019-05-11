#pragma once

#include <stdint.h>

static const char * LOAF_TASKMAN_INTERFACE_ID = "loaf_swarm_i";

static const uint16_t LOAF_TASKMAN_INTERFACE_VERSION_MAJOR = 1;
static const uint16_t LOAF_TASKMAN_INTERFACE_VERSION_MINOR = 0;

struct loaf_task_o;

struct loaf_taskman_o;

struct loaf_taskman_i
{
	loaf_taskman_o * inst;

	const uint16_t runtime_version_major;
	const uint16_t runtime_version_minor;

	void (*construct)(loaf_taskman_i * api, uint64_t task_capacity, uint16_t thread_count);

	void (*destruct)(loaf_taskman_i * api);

	loaf_task_o * (*allocate_task)(loaf_taskman_i * api, void (*task)(void *), void * data);

	loaf_task_o * (*allocate_child)(loaf_taskman_i * api, void(*task)(void *), void * data, loaf_task_o * parent);

	void (*dispatch)(loaf_taskman_i * api, loaf_task_o * task);

	void (*wait_one)(loaf_taskman_i * api, loaf_task_o * task);
};

extern "C" loaf_taskman_i * loaf_get_taskman();
