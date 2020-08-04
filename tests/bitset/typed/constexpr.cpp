#include "cases.h"
#include <gtest/gtest.h>

TYPED_TEST(CommonSubwordTypeTests, Constexpr)
{
#define AU __attribute__((unused))
	// compile-time check, that constexpr functions are usable in constexpr expression
	// Return values will be unused, flag to ignore warnings
	AU constexpr TypeParam obj_default;

	AU constexpr TypeParam obj_integer(15);

	AU constexpr TypeParam obj_array(
	    std::array<typename TypeParam::word_type, TypeParam::num_words>());

	constexpr hate::bitset<7, uint8_t> b;
	AU constexpr TypeParam obj_bitset(b);

	AU constexpr TypeParam obj_assign = obj_bitset;

	AU constexpr bool test = obj_bitset.test(7);

	AU constexpr TypeParam obj_and = obj_bitset & obj_bitset;

	AU constexpr TypeParam obj_or = obj_bitset | obj_bitset;

	AU constexpr TypeParam obj_xor = obj_bitset ^ obj_bitset;

	AU constexpr TypeParam obj_left_shift = obj_bitset << 2;

	AU constexpr TypeParam obj_right_shift = obj_bitset >> 2;

	AU constexpr TypeParam obj_invert = ~obj_bitset;

	AU constexpr auto obj_to_array = obj_bitset.to_array();

	AU constexpr bool obj_equal = obj_bitset == obj_bitset;

	AU constexpr bool obj_not_equal = obj_bitset != obj_bitset;

	AU constexpr bool obj_all = obj_bitset.all();

	AU constexpr bool obj_none = obj_bitset.none();

	AU constexpr bool obj_any = obj_bitset.any();

	AU constexpr auto obj_to_uintmax = obj_bitset.to_uintmax();

	AU constexpr auto obj_to_ulong = obj_bitset.to_ulong();

	AU constexpr auto obj_to_ullong = obj_bitset.to_ullong();
#undef AU
}
