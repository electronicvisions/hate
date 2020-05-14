#include <gtest/gtest.h>
#include "hate/memory.h"


struct A
{
	A(int a, std::string b) : m_a(a), m_b(b) {}

	int m_a;
	std::string m_b;
};


struct IndicateLifetime
{
	IndicateLifetime(bool* alive) : m_alive(alive)
	{
		*m_alive = true;
	}

	~IndicateLifetime()
	{
		*m_alive = false;
	}

	bool* m_alive;
};


TEST(Memory, MakeUniqueFromTuple)
{
	using namespace hate::memory;

	std::tuple<int, std::string> a_init = std::make_tuple(42, "foobar");

	auto a = make_unique_from_tuple<A>(a_init);

	ASSERT_EQ(a->m_a, std::get<0>(a_init));
	ASSERT_EQ(a->m_b, std::get<1>(a_init));
}

TEST(Memory, ScopedDelete)
{
	using namespace hate::memory;

	bool ptr_is_alive = false;

	{
		ASSERT_FALSE(ptr_is_alive);

		auto indicator = make_unique_from_tuple<IndicateLifetime>(std::make_tuple(&ptr_is_alive));
		ASSERT_TRUE(ptr_is_alive);

		// indicator goes out of scope
	}
	ASSERT_FALSE(ptr_is_alive);
}

TEST(Memory, MakeSharedFromTuple)
{
	using namespace hate::memory;

	std::tuple<int, std::string> a_init = std::make_tuple(42, "foobar");

	auto a = make_shared_from_tuple<A>(a_init);

	ASSERT_EQ(a->m_a, std::get<0>(a_init));
	ASSERT_EQ(a->m_b, std::get<1>(a_init));
}

TEST(Memory, ScopedDeleteShared)
{
	using namespace hate::memory;

	bool ptr_is_alive = false;

	{
		ASSERT_FALSE(ptr_is_alive);

		auto indicator = make_shared_from_tuple<IndicateLifetime>(std::make_tuple(&ptr_is_alive));
		ASSERT_TRUE(ptr_is_alive);

		// indicator goes out of scope
	}
	ASSERT_FALSE(ptr_is_alive);
}
