#pragma once

#include <type_traits>

namespace hate {

/// \brief Implementation of a trait checking for "T::iterator"
template<typename T>
class has_iterator {
	typedef char yes;
	typedef struct { char array[2]; } no;

	template<typename C> static yes test(typename C::iterator*);
	template<typename C> static no test(...);

public:
	static const bool value = sizeof(test<T>(0)) == sizeof(yes);

	typedef std::integral_constant<bool, sizeof(test<T>(0)) == sizeof(yes)> value_type;
	typedef T type;
};

/// \brief Check if some type is a specialization of another type.
/// \note This won't work correctly with non-type parameters :(.
template<typename T, template<typename...> class U>
struct is_specialization_of : std::false_type
{};

template<template<typename...> class U, typename... Args>
struct is_specialization_of<U<Args...>, U>: std::true_type
{};

} // namespace hate
