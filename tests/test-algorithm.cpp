#include "gtest/gtest.h"

#include "hate/algorithm.h"

#include <set>
#include <vector>

TEST(has_intersection, General)
{
	using namespace hate;

	std::vector<int> a = {0, 1, 2, 3, 3, 4};
	std::vector<int> b = {2, 4, 3, 5, 6};
	std::vector<int> c = {7, 8, 9};

	std::set<int> d = {0, 1, 2, 3, 4};
	std::set<int> e = {2, 4, 3, 5, 6};
	std::set<int> f = {7, 8, 9};

	EXPECT_TRUE((has_intersection(a, b)));
	EXPECT_FALSE((has_intersection(a, c)));
	EXPECT_FALSE((has_intersection(b, c)));
	EXPECT_TRUE((has_intersection(d, e)));
	EXPECT_FALSE((has_intersection(d, f)));
	EXPECT_FALSE((has_intersection(e, f)));
}
