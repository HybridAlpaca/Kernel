#pragma once

#include <atomic>
#include <stdint.h>

struct loaf_task_o
{
	void (*task)(void *);

	void * data;

	loaf_task_o * parent;

	std::atomic<uint16_t> counter;
};
