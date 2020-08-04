#include "hate/type_list.h"
#include <bitset>
#include <variant>
#include <gtest/gtest.h>


struct a { static const int my_size = 1; };
struct b { static const int my_size = 2; };
struct c { static const int my_size = 3; };
struct d { static const int my_size = 4; };
struct e { static const int my_size = 5; };
struct f { static const int my_size = 6; };
struct g { static const int my_size = 7; };
struct h { static const int my_size = 8; };
struct k { uint8_t data; uint16_t data2; };
using i = std::bitset<7>;
using j = std::bitset<3>;

using variant_1 = std::variant<a, b, c, d, e>;
using variant_2 = std::variant<f, g, h, k, i, j>;
using variant_concat_expect = std::variant<a, b, c, d, e, f, g, h, k, i, j>;


TEST(TypeList, build_alphabets)
{
	using namespace hate;
	using a_types = type_list<a, b, c>;
	using b_types = type_list<d, e>;
	using c_types = type_list<f, g, h>;
	using d_types = type_list<i, j, k>;

	using alphabet1_type = multi_concat_t<a_types>;
	EXPECT_EQ(typeid(alphabet1_type), typeid(a_types));

	using alphabet2_type = multi_concat_t<a_types, b_types>;
	EXPECT_EQ(typeid(alphabet2_type), typeid(type_list<a, b, c, d, e>));

	using alphabet3_type = multi_concat_t<a_types, b_types, c_types>;
	EXPECT_EQ(typeid(alphabet3_type), typeid(type_list<a, b, c, d, e, f, g, h>));

	using alphabet4_type = multi_concat_t<a_types, b_types, c_types, d_types>;
	EXPECT_EQ(typeid(alphabet4_type), typeid(type_list<a, b, c, d, e, f, g, h, i, j, k>));

	auto const d_in_a4_index = index_type_list_by_type<d, alphabet4_type>::value;
	EXPECT_EQ(d_in_a4_index, 3);

	// compilation fails with static_assert due to a non-existing entry
	//auto const d_in_a1_index = index_type_list_by_type<d, alphabet1_type>::value;

	// compilation fails with static_assert due to duplicate entries
	//using aa_types = type_list<a, b, c, a, b, c>;
	//auto const a_in_aa1_index = index_type_list_by_type<a, aa_types>::value;
	//auto const b_in_aa1_index = index_type_list_by_type<b, aa_types>::value;

	// compilation fails with static_assert due to out-of-bound index
	//using a_types_3_index = index_type_list_by_integer<3, a_types>::type;

	using a_types_0_index = index_type_list_by_integer<0, a_types>::type;
	EXPECT_TRUE((std::is_same<a_types_0_index, a>::value));

	auto const sizeof_a_types = type_list_size<a_types>::value;
	EXPECT_EQ(sizeof_a_types, 3);

	EXPECT_TRUE((is_in_type_list<a, a_types>::value));
	EXPECT_FALSE((is_in_type_list<d, a_types>::value));
}

namespace {

template <typename T>
struct FilterForA
{
	constexpr static bool value = !std::is_same<T, a>::value;
};

}

TEST(TypeList, filter_type_list)
{
	typedef hate::type_list<a, b, c> types_a;
	typedef hate::filter_type_list_t<FilterForA, types_a> filtered_types_a;
	EXPECT_EQ(typeid(hate::type_list<b, c>), typeid(filtered_types_a));

	typedef hate::type_list<b, c, a> types_b;
	typedef hate::filter_type_list_t<FilterForA, types_b> filtered_types_b;
	EXPECT_EQ(typeid(hate::type_list<b, c>), typeid(filtered_types_b));

	typedef hate::type_list<a> types_c;
	typedef hate::filter_type_list_t<FilterForA, types_c> filtered_types_c;
	EXPECT_EQ(typeid(hate::type_list<>), typeid(filtered_types_c));
}

TEST(TypeList, intersection)
{
	typedef hate::type_list<a, b, c> types_a;
	typedef hate::type_list<b, c, d, e, f> types_b;
	typedef hate::type_list_intersection_t<types_a, types_b> intersection;
	EXPECT_EQ(typeid(hate::type_list<b, c>), typeid(intersection));
}

TEST(TypeList, unique)
{
	typedef hate::type_list<a, b, c> types_a;
	typedef hate::type_list<a, b, c, c, b, a, f> types_b;
	typedef hate::type_list<> types_c;
	typedef hate::type_list_unique_t<types_a> unique_a;
	typedef hate::type_list_unique_t<types_b> unique_b;
	typedef hate::type_list_unique_t<types_c> unique_c;
	EXPECT_EQ(typeid(types_a), typeid(unique_a));
	EXPECT_EQ(typeid(hate::type_list<c, b, a, f>), typeid(unique_b));
	EXPECT_EQ(typeid(hate::type_list<>), typeid(unique_c));
}

TEST(TypeList, convert_from_to_variant)
{
	using type_list_1 = hate::type_list_from_t<variant_1>;
	using type_list_2 = hate::type_list_from_t<variant_2>;

	using type_list_concat = hate::concat_t<type_list_1, type_list_2>;
	using variant_concat = hate::type_list_to_t<std::variant, type_list_concat>;

	EXPECT_EQ(typeid(variant_concat), typeid(variant_concat_expect));
}
