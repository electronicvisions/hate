#pragma once

#include <array>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

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
	Timer()
		: m_start(std::chrono::duration_cast<std::chrono::nanoseconds>(
			  std::chrono::steady_clock::now().time_since_epoch())){};

	long get_s() const
	{
		auto const t = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now().time_since_epoch());
		return std::chrono::duration_cast<std::chrono::seconds>(t - m_start).count();
	};

	long get_ms() const
	{
		auto const t = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now().time_since_epoch());
		return std::chrono::duration_cast<std::chrono::milliseconds>(t - m_start).count();
	};

	long get_us() const
	{
		auto const t = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now().time_since_epoch());
		return std::chrono::duration_cast<std::chrono::microseconds>(t - m_start).count();
	};

	long get_ns() const
	{
		auto const t = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now().time_since_epoch());
		return std::chrono::duration_cast<std::chrono::nanoseconds>(t - m_start).count();
	};

	std::string print() const
	{
		auto const t = std::chrono::duration_cast<std::chrono::nanoseconds>(
		    std::chrono::steady_clock::now().time_since_epoch());
		return to_string(t - m_start);
	}

private:
	std::chrono::nanoseconds const m_start;
};
} // end hate namespace
