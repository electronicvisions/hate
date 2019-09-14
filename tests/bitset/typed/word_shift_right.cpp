#include <bitset>

#include <gtest/gtest.h>
#include "cases.h"
#include "helper.h"

TYPED_TEST(CommonSubwordTypeTests, RightShiftWords)
{
	size_t const shift = random_integer(0, TypeParam::num_words);
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	TypeParam res = lhs;
	res.shift_words_right(shift);
	// expect same behavior as std::bitset when shifting correspoding bit count
	std::string const expect =
	    (std::bitset<TypeParam::size>(lhs.to_string()) >> shift * TypeParam::num_bits_per_word)
	        .to_string();
	EXPECT_EQ(res.to_string(), expect);
}
