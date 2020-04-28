#include "gtest/gtest.h"
#include "hate/empty.h"

struct A
{
	int my_size = 1;
};

TEST(Empty, General)
{
	using namespace hate;

	EXPECT_TRUE(is_empty_v<Empty<A>>);
	EXPECT_FALSE(is_empty_v<A>);

	constexpr bool added_empty_empty = std::is_same_v<Empty<A>, add_empty_t<Empty<A>>>;
	EXPECT_TRUE(added_empty_empty);

	EXPECT_TRUE(is_empty_v<add_empty_t<Empty<A>>>);
	EXPECT_TRUE(is_empty_v<add_empty_t<A>>);

	EXPECT_FALSE(is_empty_v<remove_empty_t<A>>);
	EXPECT_FALSE(is_empty_v<remove_empty_t<Empty<A>>>);
}
