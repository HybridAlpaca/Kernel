#include <Kernel/WorkQueue.h>
#include <Kernel/Task.h>

using namespace Kernel;

WorkQueue::WorkQueue()
: bottom(0)
, top(0)
{}

WorkQueue::~WorkQueue()
{}

void WorkQueue::Push(TaskNode * node)
{
	std::scoped_lock<std::mutex> lock{criticalSection};

	taskQueue[bottom++ & QUEUE_MASK] = node;
}

TaskNode * WorkQueue::Pop()
{
	std::scoped_lock<std::mutex> lock{criticalSection};

	const uint8 taskCount = bottom - top;
	if (taskCount == 0)
	{
		// No available tasks in queue
		return nullptr;
	}

	return taskQueue[--bottom & QUEUE_MASK];
}

TaskNode * WorkQueue::Steal()
{
	std::scoped_lock<std::mutex> lock{criticalSection};

	const uint8 taskCount = bottom - top;
	if (taskCount == 0)
	{
		// No available tasks in queue
		return nullptr;
	}

	return taskQueue[++top & QUEUE_MASK];
}
