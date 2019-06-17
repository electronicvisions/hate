#include <bitset>
#include <random>

#include "gtest/gtest.h"
#include "hate/bitset.h"
#include "hate/cerealization_bitset.h"

TEST(fill_bits, General)
{
	constexpr uint32_t res = hate::fill_bits<uint32_t>(3);
	EXPECT_EQ(res, 7);
}

TEST(bitset, General)
{
	constexpr size_t size = 7; // random

	EXPECT_EQ(size, hate::bitset<size>::size);
	hate::bitset<size> bit;

	auto array1 = bit.to_array();

	EXPECT_EQ(array1, decltype(array1){0});

	bit.set(3, true);
	auto array2 = bit.to_array();
	EXPECT_EQ(array2, decltype(array2){0b0001000});

	bit.flip();
	auto array3 = bit.to_array();
	EXPECT_EQ(array3, decltype(array3){0b1110111});

	bit.set();
	auto array4 = bit.to_array();
	EXPECT_EQ(array4, decltype(array4){0b1111111});

	bit.reset(3);
	auto array5 = bit.to_array();
	EXPECT_EQ(array5, decltype(array5){0b1110111});

	bit.reset();
	auto array6 = bit.to_array();
	EXPECT_EQ(array6, decltype(array6){0});

	bit.set(2, true);
	EXPECT_TRUE(bit.test(2));
	EXPECT_FALSE(bit.test(0));

	bit.flip(0);
	EXPECT_TRUE(bit.test(0));
}

TEST(bitset, Checks)
{
	constexpr size_t size = 7;

	hate::bitset<size> bit;

	EXPECT_TRUE(bit.none());
	EXPECT_FALSE(bit.any());
	EXPECT_FALSE(bit.all());

	bit.flip();

	EXPECT_FALSE(bit.none());
	EXPECT_TRUE(bit.any());
	EXPECT_TRUE(bit.all());

	bit.reset(0);

	EXPECT_FALSE(bit.none());
	EXPECT_TRUE(bit.any());
	EXPECT_FALSE(bit.all());

	hate::bitset<size> bit2;

	bit.reset();
	EXPECT_EQ(bit, bit2);

	bit.set(1, true);
	EXPECT_FALSE(bit == bit2);
	EXPECT_TRUE(bit != bit2);

	bit2 = bit;

	EXPECT_TRUE(bit == bit2);
	EXPECT_FALSE(bit != bit2);
}

TEST(bitset, Construct)
{
	constexpr size_t size1 = 7;
	constexpr size_t size2 = 10;

	hate::bitset<size1> smaller;
	smaller.set();

	hate::bitset<size2> larger(smaller);

	EXPECT_EQ(larger.to_array(), smaller.to_array());

	hate::bitset<size2> larger2;
	larger2 = smaller;

	EXPECT_EQ(larger2.to_array(), smaller.to_array());

	hate::bitset<size2> larger3(larger);
	EXPECT_EQ(larger3, larger);

	hate::bitset<size1> from_too_large_int(255);
	EXPECT_EQ(from_too_large_int.to_array(), decltype(from_too_large_int.to_array()){0b1111111});
}

TEST(bitset, Operator)
{
	constexpr size_t size1 = 7;
	constexpr size_t size2 = 10;

	{
		hate::bitset<size1> smaller;
		smaller.set(0, true);

		hate::bitset<size2> larger;
		larger.set(7, true);

		auto res = larger | smaller;

		EXPECT_EQ(res.to_array(), decltype(res.to_array()){0b10000001});
	}

	{
		hate::bitset<size1> smaller;
		smaller.set(3, true);
		smaller.set(2, true);

		hate::bitset<size2> larger;
		larger.set(3, true);
		larger.set(7, true);

		auto res = larger & smaller;

		EXPECT_EQ(res.to_array(), decltype(res.to_array()){0b1000});
	}

	{
		hate::bitset<size1> smaller;
		smaller.set(3, true);
		smaller.set(2, true);

		hate::bitset<size2> larger;
		larger.set(3, true);
		larger.set(7, true);

		auto res = larger ^ smaller;

		EXPECT_EQ(res.to_array(), decltype(res.to_array()){0b10000100});
	}

	{
		constexpr size_t size = 35;
		hate::bitset<size> bit;
		bit.set(34, true);

		EXPECT_EQ((bit >> 34).to_array(), decltype(bit.to_array()){1});
		bit >>= 34;
		EXPECT_EQ(bit.to_array(), decltype(bit.to_array()){1});
	}

	{
		constexpr size_t size = 35;
		hate::bitset<size> bit;
		bit.set(1, true);

		EXPECT_EQ((bit << 30).to_array(), decltype(bit.to_array()){1ul << 31});
		bit <<= 30;
		EXPECT_EQ(bit.to_array(), decltype(bit.to_array()){1ul << 31});
	}

	{
		constexpr size_t size = 35;
		hate::bitset<size> bit;
		bit.set(1, true);

		EXPECT_EQ((bit << 40).to_array(), decltype(bit.to_array()){0});
		bit <<= 40;
		EXPECT_EQ(bit.to_array(), decltype(bit.to_array()){0});
	}

	{
		constexpr size_t size = 35;
		hate::bitset<size> bit;
		bit.set(1, true);

		EXPECT_EQ((bit >> 40).to_array(), decltype(bit.to_array()){0});
		bit >>= 40;
		EXPECT_EQ(bit.to_array(), decltype(bit.to_array()){0});
	}
}

TEST(bitset, SizeZero)
{
	hate::bitset<0> bit;

	hate::bitset<1> bit1;
	EXPECT_EQ(bit1 |= bit, bit1);

	bit1.set();
	EXPECT_EQ((bit1 & bit), bit1.reset());
	bit1.set();
	EXPECT_EQ((bit1 ^ bit), bit1);
}

constexpr std::array<size_t, 12> sizes{15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128, 129};

template <class T>
struct to_testing_types;

template <size_t... N>
struct to_testing_types<std::index_sequence<N...>>
{
	typedef ::testing::Types<
	    hate::bitset<sizes[N], uint8_t>...,
	    hate::bitset<sizes[N], uint16_t>...,
	    hate::bitset<sizes[N], uint32_t>...,
	    hate::bitset<sizes[N], uint64_t>...>
	    types;
};

typedef to_testing_types<std::make_index_sequence<sizes.size()>>::types Types;

template <class T>
class CommonSubwordTypeTests : public ::testing::Test
{};

TYPED_TEST_CASE(CommonSubwordTypeTests, Types);

TYPED_TEST(CommonSubwordTypeTests, DefaultConstructor)
{
	TypeParam obj;
	for (auto word : obj.to_array()) {
		EXPECT_EQ(word, 0);
	}
}

TYPED_TEST(CommonSubwordTypeTests, IntegerConstructor)
{
	size_t const top_bit = std::min((TypeParam::size - 1), sizeof(uintmax_t) * 8 - 1);
	uintmax_t const num = (1ull << top_bit) | 1ull;
	TypeParam const obj(num);
	for (size_t i = 0; i < TypeParam::size; ++i) {
		if ((i == top_bit) || (i == 0)) {
			EXPECT_TRUE(obj.test(i));
		} else {
			EXPECT_FALSE(obj.test(i));
		}
	}
}

/**
 * Fill array of integers with random values.
 * @tparam T Type of array entries
 * @tparam N Array-size
 */
template <class T, size_t N>
std::array<T, N> random_array()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<T> random;
	std::array<T, N> ret;
	for (auto& word : ret) {
		word = random(gen);
	}
	return ret;
}

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
		TypeParam const input(random_array<
		                      typename TypeParam::word_type,
		                      hate::bitset<TypeParam::size, uint8_t>::num_words>());
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

TYPED_TEST(CommonSubwordTypeTests, Assignment)
{
	// testing for same size with different alignment suffices
	// uint8_t is never WordType and therefore used as input with different alignment.
	hate::bitset<TypeParam::size, uint8_t> const obj(
	    random_array<uint8_t, hate::bitset<TypeParam::size, uint8_t>::num_words>());
	TypeParam const obj_assigned = obj;
	EXPECT_EQ(obj_assigned.to_string(), obj.to_string());
}

TYPED_TEST(CommonSubwordTypeTests, OperatorOr)
{
	{
		// test for lhs.size == rhs.size
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
		hate::bitset<TypeParam::size, typename TypeParam::word_type> const rhs(
		    random_array<typename TypeParam::word_type, TypeParam::num_words>());
		TypeParam const res = lhs | rhs;
		// expect same behavior as std::bitset
		std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) |
		                            std::bitset<TypeParam::size>(rhs.to_string()))
		                               .to_string();
		EXPECT_EQ(res.to_string(), expect);
	}
	{
		// test for lhs.size > rhs.size
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
		hate::bitset<TypeParam::size - 1, typename TypeParam::word_type> const rhs(
		    random_array<typename TypeParam::word_type, TypeParam::num_words>());
		TypeParam const res = lhs | rhs;
		// expect same behavior as std::bitset
		std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) |
		                            std::bitset<TypeParam::size>(rhs.to_string()))
		                               .to_string();
		EXPECT_EQ(res.to_string(), expect);
	}
	{
		// test for rhs.size == 0
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
		hate::bitset<0, typename TypeParam::word_type> const rhs(
		    random_array<typename TypeParam::word_type, 0>());
		TypeParam const res = lhs | rhs;
		// expect same behavior as std::bitset
		std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) |
		                            std::bitset<TypeParam::size>(rhs.to_string()))
		                               .to_string();
		EXPECT_EQ(res.to_string(), expect);
	}
}

TYPED_TEST(CommonSubwordTypeTests, OperatorAnd)
{
	{
		// test for lhs.size == rhs.size
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
		hate::bitset<TypeParam::size, typename TypeParam::word_type> const rhs(
		    random_array<typename TypeParam::word_type, TypeParam::num_words>());
		TypeParam const res = lhs & rhs;
		// expect same behavior as std::bitset
		std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) &
		                            std::bitset<TypeParam::size>(rhs.to_string()))
		                               .to_string();
		EXPECT_EQ(res.to_string(), expect);
	}
	{
		// test for lhs.size > rhs.size
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
		hate::bitset<TypeParam::size - 1, typename TypeParam::word_type> const rhs(
		    random_array<typename TypeParam::word_type, TypeParam::num_words>());
		TypeParam const res = lhs & rhs;
		// expect same behavior as std::bitset
		std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) &
		                            std::bitset<TypeParam::size>(rhs.to_string()))
		                               .to_string();
		EXPECT_EQ(res.to_string(), expect);
	}
	{
		// test for rhs.size == 0
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
		hate::bitset<0, typename TypeParam::word_type> const rhs(
		    random_array<typename TypeParam::word_type, 0>());
		TypeParam const res = lhs & rhs;
		// expect same behavior as std::bitset
		std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) &
		                            std::bitset<TypeParam::size>(rhs.to_string()))
		                               .to_string();
		EXPECT_EQ(res.to_string(), expect);
	}
}

TYPED_TEST(CommonSubwordTypeTests, OperatorXor)
{
	{
		// test for lhs.size == rhs.size
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
		hate::bitset<TypeParam::size, typename TypeParam::word_type> const rhs(
		    random_array<typename TypeParam::word_type, TypeParam::num_words>());
		TypeParam const res = lhs ^ rhs;
		// expect same behavior as std::bitset
		std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) ^
		                            std::bitset<TypeParam::size>(rhs.to_string()))
		                               .to_string();
		EXPECT_EQ(res.to_string(), expect);
	}
	{
		// test for lhs.size > rhs.size
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
		hate::bitset<TypeParam::size - 1, typename TypeParam::word_type> const rhs(
		    random_array<typename TypeParam::word_type, TypeParam::num_words>());
		TypeParam const res = lhs ^ rhs;
		// expect same behavior as std::bitset
		std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) ^
		                            std::bitset<TypeParam::size>(rhs.to_string()))
		                               .to_string();
		EXPECT_EQ(res.to_string(), expect);
	}
	{
		// test for rhs.size == 0
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
		hate::bitset<0, typename TypeParam::word_type> const rhs(
		    random_array<typename TypeParam::word_type, 0>());
		TypeParam const res = lhs ^ rhs;
		// expect same behavior as std::bitset
		std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) ^
		                            std::bitset<TypeParam::size>(rhs.to_string()))
		                               .to_string();
		EXPECT_EQ(res.to_string(), expect);
	}
}

size_t random_integer(size_t min, size_t max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> random(min, max);
	return random(gen);
}

TYPED_TEST(CommonSubwordTypeTests, OperatorLeftShift)
{
	size_t const shift = random_integer(0, TypeParam::size);
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	TypeParam const res = lhs << shift;
	// expect same behavior as std::bitset
	std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) << shift).to_string();
	EXPECT_EQ(res.to_string(), expect);
}

TYPED_TEST(CommonSubwordTypeTests, OperatorRightShift)
{
	size_t const shift = random_integer(0, TypeParam::size);
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	TypeParam const res = lhs >> shift;
	// expect same behavior as std::bitset
	std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()) >> shift).to_string();
	EXPECT_EQ(res.to_string(), expect);
}

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

TYPED_TEST(CommonSubwordTypeTests, SetComplete)
{
	// test bitset.set() sets all bits to true
	TypeParam lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	// expect same behavior as std::bitset
	std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()).set()).to_string();
	EXPECT_EQ(lhs.set().to_string(), expect);
}

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

TYPED_TEST(CommonSubwordTypeTests, ResetComplete)
{
	// test reset sets all bits to false
	TypeParam lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	// expect same behavior as std::bitset
	std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()).reset()).to_string();
	EXPECT_EQ(lhs.reset().to_string(), expect);
}

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

TYPED_TEST(CommonSubwordTypeTests, FlipComplete)
{
	// test bitset.flip() flips the value of all bits.
	TypeParam lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	// expect same behavior as std::bitset
	std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string()).flip()).to_string();
	EXPECT_EQ(lhs.flip().to_string(), expect);
}

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

TYPED_TEST(CommonSubwordTypeTests, OperatorInvert)
{
	// test operator~ inverts all bit values
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	// expect same behavior as std::bitset
	std::string const expect = (~std::bitset<TypeParam::size>(lhs.to_string())).to_string();
	EXPECT_EQ((~lhs).to_string(), expect);
}

TYPED_TEST(CommonSubwordTypeTests, Test)
{
	// test hate::bitset.test behaves as std::bitset.test
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	std::bitset<TypeParam::size> const reference(lhs.to_string());
	for (size_t i = 0; i < TypeParam::size; ++i) {
		EXPECT_EQ(lhs.test(i), reference.test(i));
	}
}

TYPED_TEST(CommonSubwordTypeTests, ToUintmax)
{
	// test hate::bitset.to_uintmax == std::bitset.to_ullong
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	std::bitset<TypeParam::size> const reference(lhs.to_string());
	EXPECT_EQ(
	    lhs.to_uintmax(),
	    (reference & decltype(reference)(~static_cast<uintmax_t>(0))).to_ullong());
}

TYPED_TEST(CommonSubwordTypeTests, ToUlong)
{
	// test hate::bitset.to_ulong == std::bitset.to_ulong
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	std::bitset<TypeParam::size> const reference(lhs.to_string());
	EXPECT_EQ(
	    lhs.to_ulong(), (reference & decltype(reference)(~static_cast<uintmax_t>(0))).to_ulong());
}

TYPED_TEST(CommonSubwordTypeTests, ToUllong)
{
	// test hate::bitset.to_ullong == std::bitset.to_ullong
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	std::bitset<TypeParam::size> const reference(lhs.to_string());
	EXPECT_EQ(
	    lhs.to_ullong(), (reference & decltype(reference)(~static_cast<uintmax_t>(0))).to_ullong());
}

TYPED_TEST(CommonSubwordTypeTests, Constexpr)
{
#define AU __attribute__((unused))
	// compile-time check, that constexpr functions are usable in constexpr expression
	// Return values will be unused, flag to ignore warnings
	AU constexpr TypeParam obj_default;

	AU constexpr TypeParam obj_integer(15);

	AU constexpr TypeParam obj_array(
	    std::array<typename TypeParam::word_type, TypeParam::num_words>());

	constexpr hate::bitset<7, uint8_t> b;
	AU constexpr TypeParam obj_bitset(b);

	AU constexpr TypeParam obj_assign = obj_bitset;

	AU constexpr bool test = obj_bitset.test(7);

	AU constexpr TypeParam obj_and = obj_bitset & obj_bitset;

	AU constexpr TypeParam obj_or = obj_bitset | obj_bitset;

	AU constexpr TypeParam obj_xor = obj_bitset ^ obj_bitset;

	AU constexpr TypeParam obj_left_shift = obj_bitset << 2;

	AU constexpr TypeParam obj_right_shift = obj_bitset >> 2;

	AU constexpr TypeParam obj_invert = ~obj_bitset;

	AU constexpr auto obj_to_array = obj_bitset.to_array();

	AU constexpr bool obj_equal = obj_bitset == obj_bitset;

	AU constexpr bool obj_not_equal = obj_bitset != obj_bitset;

	AU constexpr bool obj_all = obj_bitset.all();

	AU constexpr bool obj_none = obj_bitset.none();

	AU constexpr bool obj_any = obj_bitset.any();

	AU constexpr auto obj_to_uintmax = obj_bitset.to_uintmax();

	AU constexpr auto obj_to_ulong = obj_bitset.to_ulong();

	AU constexpr auto obj_to_ullong = obj_bitset.to_ullong();
#undef AU
}
