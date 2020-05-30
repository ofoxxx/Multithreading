#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <string>

using UDuration_ms = std::chrono::duration<double, std::milli>;

class UTestCase
{
public:
	UTestCase(const std::string& name, const std::function<bool()>& f) : m_name(name), m_case{f} {}

	bool operator()();
	operator bool() const { return m_result; }

	const std::string& name() const { return m_name; }
	const UDuration_ms& duration() const { return m_duration; }
	bool done() const { return m_duration != UDuration_ms::zero(); }
	bool successful() const { return m_result; }

private:
	std::string m_name;
	std::function<bool()> m_case;
	bool m_result { false };
	UDuration_ms m_duration { UDuration_ms::zero() };
};

extern std::ostream& operator<<(std::ostream& os, const UTestCase& tc);
