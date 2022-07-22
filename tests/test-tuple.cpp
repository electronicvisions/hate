#include "hate/tuple.h"
#include <gtest/gtest.h>

TEST(for_each, General)
{
	std::tuple<size_t, size_t, size_t> const a(0, 1, 2);
	std::tuple<int, float, int> const b(4, 5.0, 6);
	std::tuple<int, float, int> const c(5, 6.0, 7);
	std::tuple<int, float, int> const d(6, 7.0, 8);

	// check that execution order of tuple elements is ascending
	size_t expected_execution_position = 0;
	auto const function_1 = [&expected_execution_position](auto const i1) {
		bool const result = i1 == expected_execution_position;
		expected_execution_position++;
		return result;
	};
	auto const result_1 = hate::for_each(function_1, a);
	EXPECT_TRUE((std::is_same_v<decltype(result_1), std::tuple<bool, bool, bool> const>) );
	EXPECT_TRUE(std::get<0>(result_1));
	EXPECT_TRUE(std::get<1>(result_1));
	EXPECT_TRUE(std::get<2>(result_1));

	// check for two tuples
	auto const function_2 = [](auto const i1, auto const i2) { return i1 * i2; };
	auto const result_2 = hate::for_each(function_2, b, c);
	EXPECT_TRUE((std::is_same_v<decltype(result_2), std::tuple<int, float, int> const>) );
	EXPECT_EQ(std::get<0>(result_2), 20);
	EXPECT_EQ(std::get<1>(result_2), 30.0);
	EXPECT_EQ(std::get<2>(result_2), 42);

	// check for three tuples
	auto const function_3 = [](auto const i1, auto const i2, auto const i3) {
		return i1 * i2 * i3;
	};
	auto const result_3 = hate::for_each(function_3, b, c, d);
	EXPECT_TRUE((std::is_same_v<decltype(result_3), std::tuple<int, float, int> const>) );
	EXPECT_EQ(std::get<0>(result_3), 120);
	EXPECT_EQ(std::get<1>(result_3), 210.0);
	EXPECT_EQ(std::get<2>(result_3), 336);

	// check all void return type
	int counter_4 = 0;
	auto const function_4 = [&counter_4](auto const i1, auto const i2) {
		counter_4 += static_cast<int>(i1 * i2);
	};
	auto const result_4 = hate::for_each(function_4, b, c);
	EXPECT_TRUE(
	    (std::is_same_v<decltype(result_4), std::tuple<hate::Nil, hate::Nil, hate::Nil> const>) );
	EXPECT_EQ(counter_4, 92);
}
