#include "hate/type_index.h"
#include <gtest/gtest.h>

struct SomeThing
{};

template <typename T>
struct SomeTemplatedThing
{};

namespace some::where {
struct OtherThing
{};

template <typename T>
struct OtherTemplatedThing
{};
} // some::where

TEST(full_name, General)
{
	EXPECT_EQ(hate::full_name<SomeThing>(), "SomeThing");
	EXPECT_EQ(hate::full_name<some::where::OtherThing>(), "some::where::OtherThing");
	EXPECT_EQ(
	    hate::full_name<SomeTemplatedThing<some::where::OtherThing>>(),
	    "SomeTemplatedThing<some::where::OtherThing>");
	EXPECT_EQ(
	    hate::full_name<some::where::OtherTemplatedThing<some::where::OtherThing>>(),
	    "some::where::OtherTemplatedThing<some::where::OtherThing>");
	EXPECT_EQ(
	    hate::full_name<some::where::OtherTemplatedThing<some::where::OtherThing>>(),
	    "some::where::OtherTemplatedThing<some::where::OtherThing>");
}

TEST(name, General)
{
	EXPECT_EQ(hate::name<SomeThing>(), "SomeThing");
	EXPECT_EQ(hate::name<some::where::OtherThing>(), "OtherThing");
	EXPECT_EQ(
	    hate::name<SomeTemplatedThing<some::where::OtherThing>>(),
	    "SomeTemplatedThing<some::where::OtherThing>");
	EXPECT_EQ(
	    hate::name<some::where::OtherTemplatedThing<some::where::OtherThing>>(),
	    "OtherTemplatedThing<some::where::OtherThing>");
	EXPECT_EQ(
	    hate::name<some::where::OtherTemplatedThing<some::where::OtherThing>>(),
	    "OtherTemplatedThing<some::where::OtherThing>");
	EXPECT_EQ(
	    hate::name<some::where::OtherTemplatedThing<
	        some::where::OtherTemplatedThing<some::where::OtherThing>>>(),
	    "OtherTemplatedThing<some::where::OtherTemplatedThing<some::where::OtherThing> >");
}
