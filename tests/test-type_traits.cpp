#include <gtest/gtest.h>
#include "hate/type_traits.h"

typedef int base_type;

typedef ::testing::Types<
    base_type*,
    base_type**,
    base_type***,
    base_type,
    base_type const,
    base_type volatile,
    base_type const volatile,
    base_type*,
    base_type const*,
    base_type volatile*,
    base_type const volatile*,
    base_type* const,
    base_type* const volatile,
    base_type const* const,
    base_type const* const volatile,
    base_type const volatile* const,
    base_type const volatile* const volatile,
    base_type* const&,
    base_type const* const&,
    base_type const volatile* const&,
    base_type*&,
    base_type const*&,
    base_type const volatile*&,
    base_type**,
    base_type const**,
    base_type volatile**,
    base_type const volatile**,
    base_type** const,
    base_type** const volatile,
    base_type const** const,
    base_type const** const volatile,
    base_type const volatile** const,
    base_type const volatile** const volatile,
    base_type** const&,
    base_type const** const&,
    base_type const volatile** const&,
    base_type**&,
    base_type const**&,
    base_type const volatile**&,
    base_type const&,
    base_type&&,
    base_type&>
    Types;

template <class T>
class CommonTypeTests : public ::testing::Test
{};

TYPED_TEST_SUITE(CommonTypeTests, Types);

TYPED_TEST(CommonTypeTests, Types)
{
	static_assert(
	    std::is_same<hate::remove_all_qualifiers_t<TypeParam>, base_type>::value,
	    "Type is not stripped by all qualifiers.");
}
