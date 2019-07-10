#include <boost/optional.hpp>
#include "gtest/gtest.h"
#include "hate/optional.h"

// template to test comparison between variables of same base type
template <typename T>
void test_same_base_type()
{
	T no_value;
	T a = 1;
	T b = 1;
	T c = 3;

	using namespace hate;
	EXPECT_TRUE(compare_optional_equal(no_value, a, true));
	EXPECT_FALSE(compare_optional_equal(no_value, a, false));

	EXPECT_TRUE(compare_optional_equal(no_value, c, true));
	EXPECT_FALSE(compare_optional_equal(no_value, c, false));

	EXPECT_TRUE(compare_optional_equal(a, b, true));
	EXPECT_TRUE(compare_optional_equal(a, b, false));

	EXPECT_FALSE(compare_optional_equal(a, c, true));
	EXPECT_FALSE(compare_optional_equal(a, c, false));
}

// template to test comparison of variables with different base type
template <typename T1, typename T2>
void test_different_base_type()
{
	T1 no_value;
	T1 a = 1;
	T1 b = 1;
	T1 c = 3;

	T2 no_value_dif;
	T2 a_dif = 1;
	T2 b_dif = 1;
	T2 c_dif = 3;

	using namespace hate;
	EXPECT_TRUE(compare_optional_equal(no_value_dif, no_value_dif, true));
	EXPECT_FALSE(compare_optional_equal(no_value_dif, no_value_dif, false));
	EXPECT_TRUE(compare_optional_equal(a, a_dif, true));
	EXPECT_TRUE(compare_optional_equal(a, a_dif, false));
	EXPECT_TRUE(compare_optional_equal(b, b_dif, true));
	EXPECT_TRUE(compare_optional_equal(b, b_dif, false));
	EXPECT_TRUE(compare_optional_equal(c, c_dif, true));
	EXPECT_TRUE(compare_optional_equal(c, c_dif, false));

	EXPECT_TRUE(compare_optional_equal(no_value_dif, a, true));
	EXPECT_FALSE(compare_optional_equal(no_value_dif, a, false));
	EXPECT_TRUE(compare_optional_equal(no_value_dif, c, true));
	EXPECT_FALSE(compare_optional_equal(no_value_dif, c, false));
	EXPECT_TRUE(compare_optional_equal(a_dif, b, true));
	EXPECT_TRUE(compare_optional_equal(a_dif, b, false));
	EXPECT_FALSE(compare_optional_equal(a_dif, c, true));
	EXPECT_FALSE(compare_optional_equal(a_dif, c, false));

	EXPECT_TRUE(compare_optional_equal(no_value, a_dif, true));
	EXPECT_FALSE(compare_optional_equal(no_value, a_dif, false));
	EXPECT_TRUE(compare_optional_equal(no_value, c_dif, true));
	EXPECT_FALSE(compare_optional_equal(no_value, c_dif, false));
	EXPECT_TRUE(compare_optional_equal(a, b_dif, true));
	EXPECT_TRUE(compare_optional_equal(a, b_dif, false));
	EXPECT_FALSE(compare_optional_equal(a, c_dif, true));
	EXPECT_FALSE(compare_optional_equal(a, c_dif, false));
}


TEST(optional, compare_optional_equal_SameBaseType_boost)
{
	test_same_base_type<boost::optional<int>>();
	test_same_base_type<hate::optional<int>>();
}

TEST(optional, compare_optional_equal_DifferentBaseType)
{
	test_different_base_type<boost::optional<int>, boost::optional<long int>>();
	test_different_base_type<hate::optional<int>, hate::optional<long int>>();
}
