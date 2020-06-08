#include "UTestCase.h"
#include "Spinlock.h"
#include <mutex>

bool UTestCase::operator()()
{
	auto started = std::chrono::high_resolution_clock::now();
	m_result = m_case();
	m_duration = std::chrono::high_resolution_clock::now() - started;
	return m_result;
}

std::ostream& operator<<(std::ostream& os, const UTestCase& tc)
{
	static Spinlock spinlock;
	std::lock_guard<Spinlock> lock(spinlock);
	if (!tc.name().empty())
	{
		os << tc.name() << ": ";
	}
	if (tc.done())
	{
		if (tc.successful())
		{
			os << "OK (" << tc.duration().count() << " ms)\n";
		}
		else
		{
			os << "failed\n";
		}
	}
	else
	{
		os << "not performed\n";
	}
	return os;
}
