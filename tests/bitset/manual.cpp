#include "hate/bitset.h"
#include <gtest/gtest.h>

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
