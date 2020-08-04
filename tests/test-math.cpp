#include "hate/math.h"
#include <gtest/gtest.h>

TEST(NumBits, General) {
	using namespace hate::math;
	EXPECT_EQ(num_bits(0), 1);
	EXPECT_EQ(num_bits(7), 3);
	EXPECT_EQ(num_bits(8), 4);
}

TEST(round_up_integer_division, General)
{
	using namespace hate::math;
	EXPECT_EQ(round_up_integer_division(7, 7), 1);
	EXPECT_EQ(round_up_integer_division(7, 8), 1);
	EXPECT_EQ(round_up_integer_division(8, 7), 2);

	// compile-test
	__attribute__((unused)) constexpr size_t res = round_up_integer_division(10, 7);
}

TEST(round_up_to_multiple, General)
{
	using namespace hate::math;
	EXPECT_EQ(round_up_to_multiple(7, 7), 7);
	EXPECT_EQ(round_up_to_multiple(7, 8), 8);
	EXPECT_EQ(round_up_to_multiple(8, 7), 14);

	// compile-test
	__attribute__((unused)) constexpr size_t res = round_up_to_multiple(10, 7);
}

TEST(pow, General)
{
	using namespace hate::math;
	EXPECT_EQ(pow(7, 7), 823543);
	EXPECT_EQ(pow(1, 42), 1);
	EXPECT_EQ(pow(2, 1), 2);
	EXPECT_EQ(pow(2, 0), 1);

	// compile-test
	__attribute__((unused)) constexpr size_t res = pow(10, 7);
}

TEST(log2, General)
{
	using namespace hate::math;
	EXPECT_EQ(log2(1), 0);
	EXPECT_EQ(log2(7), 2);
	EXPECT_EQ(log2(8), 3);

	// compile-test
	__attribute__((unused)) constexpr size_t res = log2(7);
}
