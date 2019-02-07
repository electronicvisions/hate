#include <bitset>

#include "gtest/gtest.h"
#include "hate/math.h"

TEST(NumBits, General) {
	using namespace hate::math;
	EXPECT_EQ(num_bits(0), 1);
	EXPECT_EQ(num_bits(7), 3);
	EXPECT_EQ(num_bits(8), 4);
}
