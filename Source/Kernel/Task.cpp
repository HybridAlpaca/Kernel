#include <Kernel/Task.h>

Kernel::TaskNode * Kernel::CreateTask(Kernel::TaskProc * proc)
{
	TaskNode * node = new TaskNode{};

	node -> execute = proc;
	node -> pendingCount = 1;

	return node;
}

void Kernel::DestroyTask(Kernel::TaskNode * node)
{
	delete node;
}
