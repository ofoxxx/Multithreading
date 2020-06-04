#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include "CASQueue.h"

class DynamicPool
{
public:
	static const unsigned short LowConcurrency;
	static const unsigned short HighConcurrency;

	DynamicPool(size_t capacity = HighConcurrency);
	~DynamicPool();

	using Task = std::function<void()>;
	void enqueue(Task&&);

private:
	void work();
	void manage();

	size_t m_capacity;
	volatile bool m_stop;

	using WorkerPool = std::vector<std::thread>;
	WorkerPool m_workers;
	using DoneQueue = CASQueue<std::thread::id>;
	DoneQueue m_done;

	using TaskQueue = CASQueue<Task>;
	TaskQueue m_tasks;

	std::mutex m_manageMutex;
	std::condition_variable m_manageCondition;
	std::thread m_manager;
};
