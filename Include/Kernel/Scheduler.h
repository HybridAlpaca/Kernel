#pragma once

#include <Subframe/Subframe.h>
#include "WorkQueue.h"
#include "Task.h"

#include <atomic>
#include <thread>

namespace Kernel
{
	class Scheduler final
	{
		thread_local static WorkQueue * threadQueue;

		WorkQueue * queueList;

		std::thread * threadPool;
		std::atomic_flag done;

	public:

		Scheduler(uint16 workerThreadCount);
		~Scheduler();
	};
}
