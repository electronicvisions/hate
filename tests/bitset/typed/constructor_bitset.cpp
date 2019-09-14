#include <gtest/gtest.h>
#include "cases.h"
#include "hate/bitset.h"
#include "helper.h"

TYPED_TEST(CommonSubwordTypeTests, BitsetConstructor)
{
	// The from-bitset-constrctor has three different branches for sizeof(OtherWordType) in
	// dependence of sizeof(WordType). Every branch has to mask larger, smaller or equal-sized
	// bitset input correctly.

	// WordType > OtherWordType
	// Use uint8_t as it is smaller than all tested bitset word types
	{
		// input bitset size matches the to be constructed bitset size
		hate::bitset<TypeParam::size, uint8_t> const input(
		    random_array<uint8_t, hate::bitset<TypeParam::size, uint8_t>::num_words>());
		TypeParam const obj(input);
		EXPECT_EQ(input.to_string(), obj.to_string());
	}
	{
		// input bitset size is larger than the to be constructed bitset size
		hate::bitset<TypeParam::size + 1, uint8_t> const input(
		    random_array<uint8_t, hate::bitset<TypeParam::size + 1, uint8_t>::num_words>());
		TypeParam const obj(input);
		auto input_str = input.to_string();
		// erase highest bit of input bitset string
		input_str.erase(0, 1);
		EXPECT_EQ(input_str, obj.to_string());
	}
	{
		// input bitset size is smaller than the to be constructed bitset size
		hate::bitset<TypeParam::size - 1, uint8_t> const input(
		    random_array<uint8_t, hate::bitset<TypeParam::size - 1, uint8_t>::num_words>());
		TypeParam const obj(input);
		auto output_str = obj.to_string();
		// erase highest bit of output bitset string
		output_str.erase(0, 1);
		EXPECT_EQ(output_str, input.to_string());
	}
	// WordType == OtherWordType
	{
		// input bitset size matches the to be constructed bitset size
		TypeParam const input(
		    random_array<
		        typename TypeParam::word_type,
		        hate::bitset<TypeParam::size, typename TypeParam::word_type>::num_words>());
		TypeParam const obj(input);
		EXPECT_EQ(input.to_string(), obj.to_string());
	}
	{
		// input bitset size is larger than the to be constructed bitset size
		hate::bitset<TypeParam::size + 1, typename TypeParam::word_type> const input(
		    random_array<
		        typename TypeParam::word_type,
		        hate::bitset<TypeParam::size + 1, typename TypeParam::word_type>::num_words>());
		TypeParam const obj(input);
		auto input_str = input.to_string();
		// erase highest bit of input bitset string
		input_str.erase(0, 1);
		EXPECT_EQ(input_str, obj.to_string());
	}
	{
		// input bitset size is smaller than the to be constructed bitset size
		hate::bitset<TypeParam::size - 1, typename TypeParam::word_type> const input(
		    random_array<
		        typename TypeParam::word_type,
		        hate::bitset<TypeParam::size - 1, typename TypeParam::word_type>::num_words>());
		TypeParam const obj(input);
		auto output_str = obj.to_string();
		// erase highest bit of input bitset string
		output_str.erase(0, 1);
		EXPECT_EQ(output_str, input.to_string());
	}
	// WordType < OtherWordType (except TypeParam::word_type == uint64_t)
	{
		// input bitset size matches the to be constructed bitset size
		hate::bitset<TypeParam::size, uint64_t> const input(
		    random_array<uint64_t, hate::bitset<TypeParam::size, uint64_t>::num_words>());
		TypeParam const obj(input);
		EXPECT_EQ(input.to_string(), obj.to_string());
	}
	{
		// input bitset size is larger than the to be constructed bitset size
		hate::bitset<TypeParam::size + 1, uint64_t> const input(
		    random_array<uint64_t, hate::bitset<TypeParam::size + 1, uint64_t>::num_words>());
		TypeParam const obj(input);
		auto input_str = input.to_string();
		// erase highest bit of input bitset string
		input_str.erase(0, 1);
		EXPECT_EQ(input_str, obj.to_string());
	}
	{
		// input bitset size is smaller than the to be constructed bitset size
		hate::bitset<TypeParam::size - 1, uint64_t> const input(
		    random_array<uint64_t, hate::bitset<TypeParam::size - 1, uint64_t>::num_words>());
		TypeParam const obj(input);
		auto output_str = obj.to_string();
		// erase highest bit of input bitset string
		output_str.erase(0, 1);
		EXPECT_EQ(output_str, input.to_string());
	}
}
