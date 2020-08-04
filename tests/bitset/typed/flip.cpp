#include <bitset>

#include "cases.h"
#include "helper.h"
#include <gtest/gtest.h>

TYPED_TEST(CommonSubwordTypeTests, Flip)
{
	// test flipping the value of a single bit
	size_t const bit = random_integer(0, TypeParam::size - 1);
	TypeParam lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	// expect same behavior as std::bitset
	std::string const expect =
	    (std::bitset<TypeParam::size>(lhs.to_string()).flip(bit)).to_string();
	EXPECT_EQ(lhs.flip(bit).to_string(), expect);
}
