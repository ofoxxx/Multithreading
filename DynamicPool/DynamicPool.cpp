#include "DynamicPool.h"
#include <algorithm>

const unsigned short DynamicPool::LowConcurrency = std::thread::hardware_concurrency() / 2;
const unsigned short DynamicPool::HighConcurrency = 2 * DynamicPool::LowConcurrency - 1;

DynamicPool::DynamicPool(size_t capacity) : m_capacity(capacity), m_stop(false)
{
	m_workers.reserve(capacity);
	m_manager = std::thread(&DynamicPool::manage, this);
}

DynamicPool::~DynamicPool()
{
	m_stop = true;
	for (std::thread& worker : m_workers)
	{
		worker.join();
	}
	m_manager.join();
}

void DynamicPool::enqueue(Task&& task)
{
	if (m_stop)
		return;

	m_tasks.push_back(std::forward<Task>(task));

	m_manageCondition.notify_one();
}

void DynamicPool::work()
{
	while (auto task = m_tasks.pop_front())
	{
		(*task)();
	}
	m_done.push_back(std::this_thread::get_id());
	std::notify_all_at_thread_exit(m_manageCondition, std::unique_lock<std::mutex>(m_manageMutex));
}

void DynamicPool::manage()
{
	while (!m_stop)
	{
		std::unique_lock<std::mutex> lock(m_manageMutex);
		m_manageCondition.wait(lock, [this] { return !m_tasks.empty() || !m_workers.empty(); });

		if (!m_done.empty() && !m_workers.empty())
		{
			while (auto done_id = m_done.pop_front())
			{
				m_workers.erase(std::find_if(m_workers.begin(), m_workers.end(), [&done_id](std::thread& t) { return t.get_id() == done_id; }));
			}
		}
		if (!m_stop && !m_tasks.empty() && m_workers.size() < m_capacity)
		{
			m_workers.emplace_back(&DynamicPool::work, this);
		}
	}
}
