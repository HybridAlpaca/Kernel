#pragma once

#include <Subframe/Subframe.h>

#include <mutex>

namespace Kernel
{
	struct TaskNode;

	/**
	 * @class WorkQueue
	 * @author Seth Traman
	 *
	 * @brief A fixed size double ended queue for submitting tasks to be executed
	 *
	 * A single-producer, single consumer work queue.  Each worker thread 'owns'
	 * its own queue.  Whenever a piece of work is to be submitted, a thread pushes
	 * it to it's own queue, and may later pop it from it's own queue.  Alternatively,
	 * a thread may decide to steal a task from another thread's queue.
	 *
	 * @warning (Performance) Push, Pop, and Steal use mutex locks
	 * @todo Make this lock-free using C++11 atomics
	 **/

	class WorkQueue final
	{
		static constexpr const uint16 QUEUE_SIZE = 1024;
		static constexpr const uint16 QUEUE_MASK = (QUEUE_SIZE - 1u);

		/// Actual queue buffer where tasks are stored
		TaskNode * taskQueue[QUEUE_SIZE];

		/// Bottom index of the queue.  This grows up as items are pushed, and down as they are popped.
		uint16 bottom;

		/// Top index of the queue.  This grows up as items are stolen.
		uint16 top;

		/// Critical section used for locking.
		/// @todo Make this class lock-free
		std::mutex criticalSection;

	public:

		WorkQueue();
		WorkQueue(const WorkQueue &) = delete;
		WorkQueue(WorkQueue &&)      = delete;
		~WorkQueue();

		WorkQueue & operator=(const WorkQueue &) = delete;
		WorkQueue & operator=(WorkQueue &&)      = delete;

		/// Push a task to the top of the queue
		/// @note This is only to be called from the 'owning' thread
		void Push(TaskNode * node);

		/// Pop a task node from the bottom of the queue
		/// @note This is only to be called from the 'owning' thread
		TaskNode * Pop();

		/// Steal some work from the top of the queue
		/// @note Used by other threads to 'steal' work from this queue
		TaskNode * Steal();

		inline bool Empty()   const { return top >= bottom; }
		inline uint16 Size()  const { return bottom - top; }
	};
}
