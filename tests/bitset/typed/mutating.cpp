#include <bitset>

#include <gtest/gtest.h>
#include "cases.h"
#include "helper.h"

#define TEST_BITSET_MUTATING_COMPLETE(Func)                                                        \
	{                                                                                              \
		TypeParam lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());        \
		std::string const expect =                                                                 \
		    (std::bitset<TypeParam::size>(lhs.to_string()).Func()).to_string();                    \
		EXPECT_EQ(lhs.Func().to_string(), expect);                                                 \
	}

TYPED_TEST(CommonSubwordTypeTests, SetResetFlip)
{
	TEST_BITSET_MUTATING_COMPLETE(set)
	TEST_BITSET_MUTATING_COMPLETE(reset)
	TEST_BITSET_MUTATING_COMPLETE(flip)
}
