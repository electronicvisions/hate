#include <bitset>
#include <gtest/gtest.h>
#include "cases.h"
#include "helper.h"

TYPED_TEST(CommonSubwordTypeTests, Convert)
{
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	std::bitset<TypeParam::size> const reference(lhs.to_string());

	// test hate::bitset.to_uintmax == std::bitset.to_ullong
	EXPECT_EQ(
	    lhs.to_uintmax(),
	    (reference & decltype(reference)(~static_cast<uintmax_t>(0))).to_ullong());

	// test hate::bitset.to_ulong == std::bitset.to_ulong
	EXPECT_EQ(
	    lhs.to_ulong(), (reference & decltype(reference)(~static_cast<uintmax_t>(0))).to_ulong());

	// test hate::bitset.to_ullong == std::bitset.to_ullong
	EXPECT_EQ(
	    lhs.to_ullong(), (reference & decltype(reference)(~static_cast<uintmax_t>(0))).to_ullong());
}
