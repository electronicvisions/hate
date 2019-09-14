#include <bitset>

#include <gtest/gtest.h>
#include "cases.h"
#include "helper.h"

TYPED_TEST(CommonSubwordTypeTests, Test)
{
	// test hate::bitset.test behaves as std::bitset.test
	TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());
	std::bitset<TypeParam::size> const reference(lhs.to_string());
	for (size_t i = 0; i < TypeParam::size; ++i) {
		EXPECT_EQ(lhs.test(i), reference.test(i));
	}
}
