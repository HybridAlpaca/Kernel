#pragma once

#include <Subframe/Subframe.h>

#include <atomic>

/// Create worker threads
/// Associate queues with threads

namespace Kernel
{
	/// Singular function or unit of work to be executed as part of a task
	using TaskProc = void (const void *);

	/**
	 * @struct TaskNode
	 *
	 * @brief The smallest singular piece of work capable of being executed by a worker thread
	 * @note Aligned to a full CPU cache line to prevent false sharing between threads
	 **/

	struct alignas(8) TaskNode final
	{
		/// The function to be executed as part of the task
		TaskProc * execute;

		/// Optional parent task that 'owns' this one
		TaskNode * parent;

		/// Pending tasks (this + children, if any) that require completion
		/// @note A task is considered complete when this value reaches zero
		std::atomic<uint8> pendingCount;
	};

	/// Method of creating tasks
	TaskNode * CreateTask(TaskProc * proc);

	/// Method of destroying tasks
	void DestroyTask(TaskNode * node);
}
