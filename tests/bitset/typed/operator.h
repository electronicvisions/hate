#pragma once
#include <bitset>

#include "cases.h"
#include "helper.h"

#define OPERATOR_BINARY(Op, Name)                                                                  \
	TYPED_TEST(CommonSubwordTypeTests, Operator##Name)                                             \
	{                                                                                              \
		{                                                                                          \
			TypeParam const lhs(                                                                   \
			    random_array<typename TypeParam::word_type, TypeParam::num_words>());              \
			hate::bitset<TypeParam::size, typename TypeParam::word_type> const rhs(                \
			    random_array<typename TypeParam::word_type, TypeParam::num_words>());              \
			TypeParam const res = lhs Op rhs;                                                      \
			std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string())              \
			                                Op std::bitset<TypeParam::size>(rhs.to_string()))      \
			                               .to_string();                                           \
			EXPECT_EQ(res.to_string(), expect);                                                    \
		}                                                                                          \
		{                                                                                          \
			TypeParam const lhs(                                                                   \
			    random_array<typename TypeParam::word_type, TypeParam::num_words>());              \
			hate::bitset<TypeParam::size - 1, typename TypeParam::word_type> const rhs(            \
			    random_array<typename TypeParam::word_type, TypeParam::num_words>());              \
			TypeParam const res = lhs Op rhs;                                                      \
			std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string())              \
			                                Op std::bitset<TypeParam::size>(rhs.to_string()))      \
			                               .to_string();                                           \
			EXPECT_EQ(res.to_string(), expect);                                                    \
		}                                                                                          \
		{                                                                                          \
			TypeParam const lhs(                                                                   \
			    random_array<typename TypeParam::word_type, TypeParam::num_words>());              \
			hate::bitset<0, typename TypeParam::word_type> const rhs(                              \
			    random_array<typename TypeParam::word_type, 0>());                                 \
			TypeParam const res = lhs Op rhs;                                                      \
			std::string const expect = (std::bitset<TypeParam::size>(lhs.to_string())              \
			                                Op std::bitset<TypeParam::size>(rhs.to_string()))      \
			                               .to_string();                                           \
			EXPECT_EQ(res.to_string(), expect);                                                    \
		}                                                                                          \
	}

#define OPERATOR_SHIFT(Op, Name)                                                                   \
	TYPED_TEST(CommonSubwordTypeTests, Operator##Name)                                             \
	{                                                                                              \
		size_t const shift = random_integer(0, TypeParam::size);                                   \
		TypeParam const lhs(random_array<typename TypeParam::word_type, TypeParam::num_words>());  \
		TypeParam const res = lhs Op shift;                                                        \
		std::string const expect =                                                                 \
		    (std::bitset<TypeParam::size>(lhs.to_string()) Op shift).to_string();                  \
		EXPECT_EQ(res.to_string(), expect);                                                        \
	}
