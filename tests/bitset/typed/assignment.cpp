#include "cases.h"
#include "hate/bitset.h"
#include "helper.h"
#include <gtest/gtest.h>

TYPED_TEST(CommonSubwordTypeTests, Assignment)
{
	// testing for same size with different alignment suffices
	// uint8_t is never WordType and therefore used as input with different alignment.
	hate::bitset<TypeParam::size, uint8_t> const obj(
	    random_array<uint8_t, hate::bitset<TypeParam::size, uint8_t>::num_words>());
	TypeParam const obj_assigned = obj;
	EXPECT_EQ(obj_assigned.to_string(), obj.to_string());
}
