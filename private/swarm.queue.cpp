#include "swarm.queue.h"
#include "swarm.task.h"

#include <atomic>
#include <mutex>
#include <stdio.h>

struct swarm_queue_o
{
	std::mutex critical_section;

	loaf_task_o ** buffer;

	uint64_t capacity;

	std::atomic<uint64_t> top;
	std::atomic<uint64_t> bottom;
};

swarm_queue_o * swarm_queue_create(uint64_t capacity)
{
	auto queue = new swarm_queue_o{};

	queue->capacity = capacity;

	queue->buffer = new loaf_task_o * [capacity];

	return queue;
}

void swarm_queue_destroy(swarm_queue_o * queue)
{
	delete[] queue->buffer;

	delete queue;
}

void swarm_queue_push(swarm_queue_o * queue, loaf_task_o * task)
{
	const auto bottom = queue->bottom.load();

	queue->buffer[bottom & (queue->capacity - 1)] = task;

	// Make sure we increment queue->bottom AFTER inserting the task,
	// so that other threads don't read tasks which don't exist yet.

	queue->bottom.fetch_add(1, std::memory_order_release);
}

loaf_task_o * swarm_queue_pop(swarm_queue_o * queue)
{
	const auto bottom = (--queue->bottom);

	const auto top = queue->top.load(std::memory_order_release);

	if (top <= bottom)
	{
		auto task = queue->buffer[bottom & (queue->capacity - 1)];

		if (top != bottom)
		{
			return task;
		}

		auto expected = top;

		if (!queue->top.compare_exchange_strong(expected, top + 1))
		{
			task = nullptr;
		}

		queue->bottom = top + 1;

		return task;
	}

	queue->bottom = top;

	return nullptr;
}

loaf_task_o * swarm_queue_steal(swarm_queue_o * queue)
{
	const auto top = queue->top.load(std::memory_order_relaxed);

	const auto bottom = queue->bottom.load(std::memory_order_release);

	if (top < bottom)
	{
		auto task = queue->buffer[top & (queue->capacity - 1)];

		auto expected = top;

		if (!queue->top.compare_exchange_strong(expected, top + 1))
		{
			// queue->top was not equal to its old value, meaning a concurrent steal
			// operation removed an element from the queue in the meantime.

			// @todo: Retry?

			return nullptr;
		}

		return task;
	}

	return nullptr;
}
