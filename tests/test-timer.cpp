#include <thread>

#include "gtest/gtest.h"

#include "hate/timer.h"

TEST(Timer, General)
{
	hate::Timer t;
	size_t wait_time = 1000;
	std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
	size_t ms = t.get_ms();
	size_t s = t.get_s();
	//nano/micro second to precise for reproducible test
	EXPECT_GE(ms, wait_time);
	EXPECT_LE(ms, wait_time * 1.1);
	EXPECT_EQ(s, wait_time / 1000);
}
