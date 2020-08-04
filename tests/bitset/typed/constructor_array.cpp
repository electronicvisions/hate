#include "cases.h"
#include "hate/bitset.h"
#include "helper.h"
#include <array>
#include <gtest/gtest.h>

TYPED_TEST(CommonSubwordTypeTests, ArrayConstructor)
{
	{
		// construction from equal sized array
		auto const input = random_array<typename TypeParam::word_type, TypeParam::num_words>();
		TypeParam const obj(input);

		auto output = input;
		if (TypeParam::size % TypeParam::num_bits_per_word) {
			// Mask bits in highest array word to match bitset-size
			output.back() &= hate::fill_bits<typename TypeParam::word_type>(
			    TypeParam::size % TypeParam::num_bits_per_word);
		}
		EXPECT_EQ(obj.to_array(), output);
	}
	{
		// construction from larger array
		auto const input = random_array<typename TypeParam::word_type, TypeParam::num_words + 1>();
		TypeParam const obj(input);

		// Construct expected array content of bitset
		std::array<typename TypeParam::word_type, TypeParam::num_words> output;
		for (size_t i = 0; i < output.size(); ++i) {
			output.at(i) = input.at(i);
		}
		if (TypeParam::size % TypeParam::num_bits_per_word) {
			// Mask bits in highest array word to match bitset-size
			output.back() &= hate::fill_bits<typename TypeParam::word_type>(
			    TypeParam::size % TypeParam::num_bits_per_word);
		}
		EXPECT_EQ(obj.to_array(), output);
	}
}
