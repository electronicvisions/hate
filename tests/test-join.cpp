#include "hate/join.h"
#include <gtest/gtest.h>

#include <array>
#include <sstream>
#include <string>

TEST(Join, Array)
{
	std::array<int, 3> iterable = {1, 2, 3};
	std::string delimiter = ", ";

	{
		std::stringstream ss;
		ss << "[" << hate::join(iterable, delimiter) << "]";
		EXPECT_EQ(ss.str(), "[1, 2, 3]");
	}

	{
		std::stringstream ss;
		ss << "[" << hate::join(iterable.begin(), iterable.end(), delimiter) << "]";
		EXPECT_EQ(ss.str(), "[1, 2, 3]");
	}
}

TEST(JoinString, Array)
{
	std::array<int, 3> iterable = {1, 2, 3};
	std::string delimiter = ", ";

	{
		std::string const result = "[" + hate::join_string(iterable, delimiter) + "]";
		EXPECT_EQ(result, "[1, 2, 3]");
	}

	{
		std::string const result =
		    "[" + hate::join_string(iterable.begin(), iterable.end(), delimiter) + "]";
		EXPECT_EQ(result, "[1, 2, 3]");
	}
}
