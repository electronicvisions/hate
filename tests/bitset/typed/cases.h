#pragma once
#include "hate/bitset.h"
#include <gtest/gtest.h>

constexpr std::array<size_t, 12> sizes{15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128, 129};

template <class T>
struct to_testing_types;

template <size_t... N>
struct to_testing_types<std::index_sequence<N...>>
{
	typedef ::testing::Types<
	    hate::bitset<sizes[N], uint8_t>...,
	    hate::bitset<sizes[N], uint16_t>...,
	    hate::bitset<sizes[N], uint32_t>...,
	    hate::bitset<sizes[N], uint64_t>...>
	    types;
};

typedef to_testing_types<std::make_index_sequence<sizes.size()>>::types Types;

template <class T>
class CommonSubwordTypeTests : public ::testing::Test
{};

TYPED_TEST_SUITE(CommonSubwordTypeTests, Types);
