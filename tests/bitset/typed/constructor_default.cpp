#include "cases.h"
#include <gtest/gtest.h>

TYPED_TEST(CommonSubwordTypeTests, DefaultConstructor)
{
	TypeParam obj;
	for (auto word : obj.to_array()) {
		EXPECT_EQ(word, 0);
	}
}
