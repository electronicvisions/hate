#include "gtest/gtest.h"
#include "hate/unordered_map.h"

TEST(unordered_map, General)
{
	std::unordered_map<std::size_t, std::size_t> map;

	map.insert(std::make_pair(1, 2));
	map.insert(std::make_pair(3, 4));

	{
		auto entry = hate::get(map, 1);
		EXPECT_TRUE(entry);
		EXPECT_EQ(*entry, 2);
	}

	{
		auto const& ref = map;
		auto entry = hate::cget(ref, 3);
		EXPECT_TRUE(entry);
		EXPECT_EQ(*entry, 4);
	}

	{
		auto entry = hate::get(map, 5);
		EXPECT_FALSE(entry);
	}

	{
		auto entry = hate::get(map, 3);
		(*entry).get() = 42;

		EXPECT_EQ(map.at(3), 42);
	}
}
