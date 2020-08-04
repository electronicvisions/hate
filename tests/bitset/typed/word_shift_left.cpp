#include <bitset>

#include "cases.h"
#include "helper.h"
#include <gtest/gtest.h>

TYPED_TEST(CommonSubwordTypeTests, LeftShiftWords)
{
	size_t const shift = random_integer(0, TypeParam::num_words);
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	TypeParam res = lhs;
	res.shift_words_left(shift);
	// expect same behavior as std::bitset when shifting corresponding bit count
	std::string const expect =
	    (std::bitset<TypeParam::size>(lhs.to_string()) << shift * TypeParam::num_bits_per_word)
	        .to_string();
	EXPECT_EQ(res.to_string(), expect);
}
