#include "swarm.queue.h"
#include "swarm.task.h"
#include <taskman.h>

#include <atomic>
#include <cstdio>
#include <mutex>
#include <random>
#include <thread>

static thread_local loaf_task_o * task_pool;
static thread_local uint16_t allocated_task_count;
static thread_local uint16_t thread_id;
static uint64_t task_pool_capacity;
static swarm_queue_o ** queues;
static std::thread * thread_pool;
static uint16_t thread_pool_size;
static bool running;

static loaf_task_o * get_task_from_queue()
{
	auto & queue = queues[thread_id];

	loaf_task_o * task = swarm_queue_pop(queue);

	if (!task)
	{
		const uint16_t queue_idx = rand() % thread_pool_size;

		if (queue_idx == thread_id)
		{
			std::this_thread::yield();
			return nullptr;
		}

		return swarm_queue_steal(queues[queue_idx]);
	}

	return task;
}

static void finish_task(loaf_task_o * task)
{
	--task->counter;

	if (task->counter <= 0 && task->parent)
	{
		finish_task(task->parent);
	}
}

static void swarm_worker(uint16_t thread_idx)
{
	thread_id = thread_idx;

	task_pool = new loaf_task_o[task_pool_capacity];

	while (running)
	{
		auto task = get_task_from_queue();

		if (task)
		{
			task->task(task->data);
			finish_task(task);
		}
	}

	delete[] task_pool;
}

static void swarm_construct(loaf_taskman_i * api, uint64_t task_capacity, uint16_t thread_count)
{
	thread_id = 0;

	task_pool_capacity = task_capacity;
	thread_pool_size = thread_count;

	task_pool = new loaf_task_o[task_capacity];
	queues = new swarm_queue_o * [thread_count + 1];
	thread_pool = new std::thread[thread_count];

	running = true;

	for (auto i = 0; i < thread_count + 1; ++i)
	{
		queues[i] = swarm_queue_create(task_capacity);
	}

	for (auto i = 0; i < thread_count; ++i)
	{
		thread_pool[i] = std::thread{swarm_worker, i + 1};
	}
}

static void swarm_destruct(loaf_taskman_i * api)
{
	running = false;

	for (auto i = 0; i < thread_pool_size; ++i)
	{
		if (thread_pool[i].joinable())
		{
			thread_pool[i].join();
		}
	}

	for (auto i = 0; i < thread_pool_size; ++i)
	{
		swarm_queue_destroy(queues[i]);
	}

	delete[] thread_pool;
	delete[] queues;
	delete[] task_pool;
}

static loaf_task_o * swarm_allocate_task(loaf_taskman_i * api, void (*task)(void *), void * data)
{
	const uint16_t idx = allocated_task_count++;

	loaf_task_o * allocated = &task_pool[idx & (task_pool_capacity - 1u)];

	allocated->task = task;
	allocated->data = data;
	allocated->parent = nullptr;
	allocated->counter = 1;

	return allocated;
}

static loaf_task_o * swarm_allocate_child(loaf_taskman_i * api, void (*task)(void *), void * data, loaf_task_o * parent)
{
	const uint16_t idx = allocated_task_count++;

	loaf_task_o * allocated = &task_pool[idx & (task_pool_capacity - 1u)];

	++parent->counter;

	allocated->task = task;
	allocated->data = data;
	allocated->parent = parent;
	allocated->counter = 1;

	return allocated;
}

static void swarm_dispatch(loaf_taskman_i * api, loaf_task_o * task)
{
	swarm_queue_push(queues[thread_id], task);
}

static void swarm_wait_one(loaf_taskman_i * api, loaf_task_o * task)
{
	while (task->counter != 0)
	{
		// std::printf("%d\n", task->counter.load());

		loaf_task_o * task = get_task_from_queue();

		if (task)
		{
			task->task(task->data);
			finish_task(task);
		}
	}
}

loaf_taskman_i * loaf_get_taskman()
{
	static loaf_taskman_i taskman
	{
		nullptr,
		LOAF_TASKMAN_INTERFACE_VERSION_MAJOR,
		LOAF_TASKMAN_INTERFACE_VERSION_MINOR,
		swarm_construct,
		swarm_destruct,
		swarm_allocate_task,
		swarm_allocate_child,
		swarm_dispatch,
		swarm_wait_one
	};

	return &taskman;
}
