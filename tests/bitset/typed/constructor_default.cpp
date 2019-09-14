#include <gtest/gtest.h>
#include "cases.h"

TYPED_TEST(CommonSubwordTypeTests, DefaultConstructor)
{
	TypeParam obj;
	for (auto word : obj.to_array()) {
		EXPECT_EQ(word, 0);
	}
}
