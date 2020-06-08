#pragma once

#include <atomic>

class Spinlock
{
public:
	Spinlock() : m_lock{ATOMIC_FLAG_INIT} {};

	void lock()
	{
		while (m_lock.test_and_set(std::memory_order_acquire));
	}

	void unlock()
	{
		m_lock.clear(std::memory_order_release);
	}

private:
	std::atomic_flag m_lock;
};
