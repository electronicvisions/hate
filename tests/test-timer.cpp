#include "hate/timer.h"
#include <thread>
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
