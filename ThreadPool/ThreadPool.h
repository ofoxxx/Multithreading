#pragma once

#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
	static const unsigned short LowConcurrency;
	static const unsigned short HighConcurrency;

	ThreadPool(size_t capacity = HighConcurrency);
	~ThreadPool();

	using Task = std::function<void()>;
	void enqueue(Task&&);

private:
	void worker();
	bool m_stop;

	using WorkerPool = std::vector<std::thread>;
	WorkerPool m_workers;

	using TaskQueue = std::queue<Task, std::list<Task>>;
	TaskQueue m_tasks;

	std::mutex m_taskMutex;
	std::condition_variable m_runCondition;
};
