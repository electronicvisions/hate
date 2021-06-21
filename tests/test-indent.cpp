#include "hate/indent.h"
#include <gtest/gtest.h>

#include <sstream>
#include <string>

TEST(Indent, Tab)
{
	std::stringstream ss;
	ss << "first line\n";
	ss << "second line\n";
	ss << "third line";
	auto const result = hate::indent(ss.str(), "\t");

	std::stringstream ex;
	ex << "\tfirst line\n";
	ex << "\tsecond line\n";
	ex << "\tthird line";
	EXPECT_EQ(result, ex.str());
}
