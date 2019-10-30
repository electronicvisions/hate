#include <gtest/gtest.h>
#include "hate/history_wrapper.h"

using namespace hate;

/** Test structure used as HistoryWrapper value_type. */
struct TestStruct
{
	int value;

	TestStruct(int const value = 0) : value(value) {}
	bool operator==(TestStruct const& other) const { return value == other.value; }
};

TEST(HistoryWrapper, General)
{
	EXPECT_NO_THROW(HistoryWrapper<TestStruct>());
	EXPECT_EQ(HistoryWrapper<TestStruct>::depth, 1);

	HistoryWrapper<TestStruct> history;

	// no history
	EXPECT_FALSE(history.has_history());
	EXPECT_FALSE(history.has_history(0));

	// history inaccessible
	EXPECT_THROW(history.get_history(), std::runtime_error);
	EXPECT_THROW(history.get_history(0), std::runtime_error);

	EXPECT_EQ(*history, TestStruct());
	EXPECT_EQ(history->value, 0);

	history->value = 3;
	EXPECT_EQ(history->value, 3);
	(*history).value = 4;
	EXPECT_EQ(history->value, 4);

	// alter via reference
	auto& value = *history;
	value.value = 3;
	EXPECT_EQ(history->value, 3);
	EXPECT_EQ(*history, TestStruct(3));

	// save, expect reset touched, history present with former current value, current value
	// preserved
	history.save();
	EXPECT_EQ(*history, TestStruct(3));
	EXPECT_TRUE(history.has_history());
	EXPECT_EQ(history.get_history(), TestStruct(3));

	history->value = 2;
	EXPECT_EQ(*history, TestStruct(2));
	EXPECT_EQ(history.get_history(), TestStruct(3));

	// history with depth > 1
	HistoryWrapper<TestStruct, 2> multi_history;
	EXPECT_FALSE(multi_history.has_history(0));
	EXPECT_FALSE(multi_history.has_history(1));

	multi_history.save();
	multi_history->value = 1;
	EXPECT_TRUE(multi_history.has_history(0));
	EXPECT_FALSE(multi_history.has_history(1));
	EXPECT_EQ(*multi_history, TestStruct(1));
	EXPECT_EQ(multi_history.get_history(0), TestStruct());

	multi_history.save();
	multi_history->value = 2;
	EXPECT_TRUE(multi_history.has_history(0));
	EXPECT_TRUE(multi_history.has_history(1));
	EXPECT_EQ(*multi_history, TestStruct(2));
	EXPECT_EQ(multi_history.get_history(0), TestStruct(1));
	EXPECT_EQ(multi_history.get_history(1), TestStruct());

	// discard of oldest
	multi_history.save();
	multi_history->value = 3;
	EXPECT_TRUE(multi_history.has_history(0));
	EXPECT_TRUE(multi_history.has_history(1));
	EXPECT_EQ(*multi_history, TestStruct(3));
	EXPECT_EQ(multi_history.get_history(0), TestStruct(2));
	EXPECT_EQ(multi_history.get_history(1), TestStruct(1));
}
