#include "cases.h"
#include <gtest/gtest.h>

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
