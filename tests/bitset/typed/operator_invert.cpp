#include <bitset>
#include <gtest/gtest.h>
#include "cases.h"
#include "helper.h"

TYPED_TEST(CommonSubwordTypeTests, OperatorInvert)
{
	// test operator~ inverts all bit values
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	// expect same behavior as std::bitset
	std::string const expect = (~std::bitset<TypeParam::size>(lhs.to_string())).to_string();
	EXPECT_EQ((~lhs).to_string(), expect);
}
