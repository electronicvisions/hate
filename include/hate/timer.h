#pragma once

#include <chrono>

namespace hate {

class Timer
{
public:
	Timer()
		: m_start(std::chrono::duration_cast<std::chrono::nanoseconds>(
			  std::chrono::steady_clock::now().time_since_epoch())){};

	size_t get_s() const
	{
		auto const t = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now().time_since_epoch());
		return std::chrono::duration_cast<std::chrono::seconds>(t - m_start).count();
	};

	size_t get_ms() const
	{
		auto const t = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now().time_since_epoch());
		return std::chrono::duration_cast<std::chrono::milliseconds>(t - m_start).count();
	};

	size_t get_us() const
	{
		auto const t = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now().time_since_epoch());
		return std::chrono::duration_cast<std::chrono::microseconds>(t - m_start).count();
	};

	size_t get_ns() const
	{
		auto const t = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now().time_since_epoch());
		return std::chrono::duration_cast<std::chrono::nanoseconds>(t - m_start).count();
	};

private:
	std::chrono::nanoseconds const m_start;
};
} // end hate namespace
