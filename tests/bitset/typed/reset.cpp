#include <bitset>

#include "cases.h"
#include "helper.h"
#include <gtest/gtest.h>

TYPED_TEST(CommonSubwordTypeTests, Reset)
{
	// test resetting single bit to false
	size_t const bit = random_integer(0, TypeParam::size - 1);
	TypeParam lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	// expect same behavior as std::bitset
	std::string const expect =
	    (std::bitset<TypeParam::size>(lhs.to_string()).reset(bit)).to_string();
	EXPECT_EQ(lhs.reset(bit).to_string(), expect);
}
