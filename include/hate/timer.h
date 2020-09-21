#pragma once

#include <array>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

extern "C"
{
#include <sys/time.h>
}

namespace hate {

/**
 * Convert duration with specified minimal precision for nano-, micro-, milli- or seconds to string.
 * The correct metric prefix is chosen for the unit print.
 * The precision states the minimal number of digits to be printed, only above 1000s the number of
 * digits is increased. Not printed characters are discarded without looking at rounding.
 * @tparam precision Number of digits to print (minimally, defaults to 3 to be aligned with metric
 * prefix steps)
 * @tparam Rep Representation type used in duration
 * @tparam Period Period specifier of duration
 * @param value Duration value to print
 */
template <int Precision = 3, typename Rep, typename Period>
std::string to_string(std::chrono::duration<Rep, Period> const& value)
{
	// convert to nanoseconds
	auto const ns = std::chrono::duration_cast<std::chrono::nanoseconds>(value);
	typedef typename decltype(ns)::rep ns_rep;
	ns_rep const ns_count = ns.count();

	constexpr static std::array<ns_rep, 4> thousands = {
	    1, static_cast<ns_rep>(1e3), static_cast<ns_rep>(1e6), static_cast<ns_rep>(1e9)};
	static const std::array<std::string, 4> names = {"ns", "us", "ms", "s"};

	// calculate unit to choose such that integer value is >= 1
	auto const log1000 = [](ns_rep const val) -> size_t {
		auto const log1000_impl = [](size_t const l, ns_rep const val, auto& impl) -> size_t {
			return val < 1000 ? l : impl(l + 1, val / 1000, impl);
		};
		return log1000_impl(0, val, log1000_impl);
	};
	size_t const n_thousand = std::min(log1000(ns_count), thousands.size() - 1);

	double const val = double(ns_count) / double(thousands[n_thousand]);
	std::stringstream ss;
	ss << std::setprecision(Precision) << val << " " << names[n_thousand];

	return ss.str();
}


class Timer
{
public:
	Timer();

	long get_s() const;

	long get_ms() const;

	long get_us() const;

	long get_ns() const;

	std::string print() const;

private:
	std::chrono::microseconds m_start;

	static std::chrono::microseconds fast_now()
	{
		timeval now;
		gettimeofday(&now, nullptr);
		return std::chrono::microseconds{now.tv_sec * 1000 * 1000 + now.tv_usec};
	}
};

inline Timer::Timer() : m_start(Timer::fast_now()) {}

inline long Timer::get_s() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(Timer::fast_now() - m_start).count();
}

inline long Timer::get_ms() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(Timer::fast_now() - m_start)
	    .count();
}

inline long Timer::get_us() const
{
	return std::chrono::duration_cast<std::chrono::microseconds>(Timer::fast_now() - m_start)
	    .count();
}

inline long Timer::get_ns() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(Timer::fast_now() - m_start)
	    .count();
}

inline std::string Timer::print() const
{
	std::chrono::microseconds dt{fast_now() - m_start};
	return to_string(dt);
}

} // end hate namespace
