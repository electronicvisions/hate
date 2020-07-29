#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include "hate/track_modifications.h"

using namespace hate;
using namespace std::chrono_literals;

TEST(TrackModifications, General)
{
	TrackModifications<int> tracked1;
	[[maybe_unused]] auto tracked2 = tracked1;
	auto tracked3 = decltype(tracked1)(std::move(tracked1));

	tracked1 = TrackModifications<int>(42);
	ASSERT_FALSE(tracked3.is_elapsed(10s));

	std::this_thread::sleep_for(100ms);
	std::ignore = *tracked3 + 42;
	ASSERT_TRUE(tracked3.is_elapsed(100ms));

	tracked1.get() += 1;
	ASSERT_FALSE(tracked1.is_elapsed(1s));

	std::this_thread::sleep_for(100ms);
	ASSERT_TRUE(tracked1.elapsed() >= 100ms);
}
