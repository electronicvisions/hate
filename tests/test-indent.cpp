#include "hate/indent.h"
#include <gtest/gtest.h>

#include "hate/timer.h"
#include <sstream>
#include <string>

TEST(IndentingOstream, Tab)
{
	std::stringstream ss;
	hate::IndentingOstream iss(ss);
	iss << hate::Indentation("\t") << "first line\n"
	    << "second line\n"
	    << "third line";
	auto const result = ss.str();

	std::stringstream ex;
	ex << "\tfirst line\n";
	ex << "\tsecond line\n";
	ex << "\tthird line";
	EXPECT_EQ(result, ex.str());
}

TEST(IndentingOstream, IndentChange)
{
	std::stringstream ss;
	hate::IndentingOstream iss(ss);
	iss << hate::Indentation("\t") << "first line\n"
	    << "second line\n"
	    << hate::Indentation("\t\t") << "third line";
	auto const result = ss.str();
	std::stringstream ex;
	ex << "\tfirst line\n";
	ex << "\tsecond line\n";
	ex << "\t\tthird line";
	EXPECT_EQ(result, ex.str());
}

TEST(IndentingOstream, Performance)
{
	constexpr size_t num = 1000;

	hate::Timer timer_indenting_ostream;
	std::stringstream ss;
	hate::IndentingOstream iss(ss);
	iss << hate::Indentation("\t");

	for (size_t i = 0; i < num; ++i) {
		iss << "first line with a lot of characters which we don't really care about but they "
		       "are "
		       "still "
		       "needed.\n";
	}
	auto const result = ss.str();
	EXPECT_LE(timer_indenting_ostream.get_ms(), 10);
}
