#include "ThreadPool.h"

const unsigned short ThreadPool::LowConcurrency = std::thread::hardware_concurrency() / 2;
const unsigned short ThreadPool::HighConcurrency = 2 * ThreadPool::LowConcurrency - 1;

ThreadPool::ThreadPool(size_t capacity) : m_stop(false)
{
	m_workers.reserve(capacity);
	for (size_t i = 0; i < capacity; ++i)
	{
		m_workers.emplace_back(&ThreadPool::worker, this);
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(m_taskMutex);
		m_stop = true;
	}
	m_runCondition.notify_all();
	for (std::thread& worker : m_workers)
	{
		worker.join();
	}
}

void ThreadPool::enqueue(Task&& task)
{
	std::unique_lock<std::mutex> lock(m_taskMutex);
	if (m_stop)
		return;
	m_tasks.push(std::forward<Task>(task));
	m_runCondition.notify_one();
}

void ThreadPool::worker()
{
	while (true)
	{
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(m_taskMutex);
			m_runCondition.wait(lock, [this] { return m_stop || !m_tasks.empty(); });
			if (m_tasks.empty())
				return;
			task = std::move(m_tasks.front());
			m_tasks.pop();
		}
		task();
	}
}
