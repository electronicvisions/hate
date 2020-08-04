#include "hate/type_index.h"
#include <gtest/gtest.h>

struct SomeThing
{};

namespace some::where {
struct OtherThing
{};
} // some::where

TEST(full_name, General)
{
	EXPECT_EQ(hate::full_name<SomeThing>(), "SomeThing");
	EXPECT_EQ(hate::full_name<some::where::OtherThing>(), "some::where::OtherThing");
}

TEST(name, General)
{
	EXPECT_EQ(hate::name<SomeThing>(), "SomeThing");
	EXPECT_EQ(hate::name<some::where::OtherThing>(), "OtherThing");
}
