#include "hate/join.h"
#include <gtest/gtest.h>

#include <array>
#include <sstream>
#include <string>

TEST(JoinString, Array)
{
	std::array<int, 3> iterable = {1, 2, 3};
	std::string delimiter = ", ";

	std::stringstream ss;
	ss << "[" << hate::join_string(iterable, delimiter) << "]";
	EXPECT_EQ(ss.str(), "[1, 2, 3]");
}

TEST(Join, Array)
{
	std::array<int, 3> iterable = {1, 2, 3};
	std::string delimiter = ", ";

	std::stringstream ss;
	ss << "[";
	hate::join(ss, iterable, delimiter);
	ss << "]";
	EXPECT_EQ(ss.str(), "[1, 2, 3]");
}
