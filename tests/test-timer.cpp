#include "hate/timer.h"
#include <future>
#include <iostream>
#include <thread>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <gtest/gtest.h>

TEST(PrintDuration, General)
{
	{
		std::chrono::seconds dur(12345);
		EXPECT_EQ(hate::to_string(dur), "1.23e+04 s");
	}
	{
		std::chrono::milliseconds dur(123);
		EXPECT_EQ(hate::to_string(dur), "123 ms");
	}
	{
		std::chrono::milliseconds dur(1234);
		EXPECT_EQ(hate::to_string(dur), "1.23 s");
	}
	{
		std::chrono::microseconds dur(1236);
		EXPECT_EQ(hate::to_string(dur), "1.24 ms");
	}
	{
		std::chrono::microseconds dur(126);
		EXPECT_EQ(hate::to_string(dur), "126 us");
	}
	{
		std::chrono::nanoseconds dur(12);
		EXPECT_EQ(hate::to_string(dur), "12 ns");
		EXPECT_EQ(hate::to_string<2>(dur), "12 ns");
	}
}

TEST(Timer, General)
{
	hate::Timer t;
	size_t wait_time = 1000;
	std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
	size_t ns = t.get_ns();
	size_t us = t.get_us();
	size_t ms = t.get_ms();
	size_t s = t.get_s();
	EXPECT_EQ(t.print(), "1 s");
	EXPECT_GE(ns, wait_time * 1000 * 1000);
	EXPECT_LE(ns, wait_time * 1000 * 1000 * 1.1);
	EXPECT_GE(us, wait_time * 1000);
	EXPECT_LE(us, wait_time * 1000 * 1.1);
	EXPECT_GE(ms, wait_time);
	EXPECT_LE(ms, wait_time * 1.1);
	EXPECT_EQ(s, wait_time / 1000);
}

/**
 * This test measures the overhead of the Timer by measuring the amount of time of multiple
 * back-to-back measurements. It does not assert an expectation but prints the measured overhead per
 * single use of Timer.
 */
TEST(Timer, Overhead)
{
	size_t const num_threads = std::thread::hardware_concurrency();
	std::cout << "number of threads: " << num_threads << std::endl;
	std::array<size_t, 4> nums = {10, 100, 1000, 10000};
	boost::accumulators::accumulator_set<
	    double, boost::accumulators::features<
	                boost::accumulators::tag::mean, boost::accumulators::tag::variance>>
	    acc;
	for (auto const num : nums) {
		for (size_t n = 0; n < 100; ++n) {
			std::vector<long> repeat_duration_us;
			auto const repeat_timer = [num]() {
				hate::Timer to;
				for (size_t i = 0; i < num; ++i) {
					hate::Timer t;
					[[maybe_unused]] auto volatile end = t.get_us();
				}
				return to.get_us();
			};
			std::vector<std::future<long>> concurrent_repeats;
			for (size_t i = 0; i < num_threads; ++i) {
				concurrent_repeats.push_back(std::async(std::launch::async, repeat_timer));
			}
			for (auto& repeat : concurrent_repeats) {
				acc(static_cast<double>(repeat.get()) / num);
			}
		}
		std::cout << num << " back-to-back measurements: mean(" << boost::accumulators::mean(acc)
		          << " us), std(" << std::sqrt(boost::accumulators::variance(acc)) << " us)"
		          << std::endl;
	}
	EXPECT_LE(boost::accumulators::mean(acc), 0.5);
}
