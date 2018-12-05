#include <Kernel/Scheduler.h>

#include <iostream>

using namespace Kernel;

// Create queues
// Set local queue
// Spawn threads

Scheduler::Scheduler(uint16 workerThreadCount)
{
	std::cout << "[Thread 0] Creating master queue & thread pool" << '\n';

	queueList  = new WorkQueue[workerThreadCount + 1]; // Add 1 for main thread
	threadPool = new std::thread[workerThreadCount];
}


Scheduler::~Scheduler()
{
	//
}
