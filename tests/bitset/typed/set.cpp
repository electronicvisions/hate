#include <bitset>

#include <gtest/gtest.h>
#include "cases.h"
#include "helper.h"

TYPED_TEST(CommonSubwordTypeTests, Set)
{
	// test setting single bit to true
	size_t const bit = random_integer(0, TypeParam::size - 1);
	TypeParam lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	// expect same behavior as std::bitset
	std::string const expect =
	    (std::bitset<TypeParam::size>(lhs.to_string()).set(bit, true)).to_string();
	EXPECT_EQ(lhs.set(bit, true).to_string(), expect);
}
