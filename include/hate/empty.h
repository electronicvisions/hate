#pragma once
#include <type_traits>

namespace hate {

/**
 * Empty structure wrapping type.
 * @tparam T Type to wrap
 */
template <typename T>
struct Empty
{
	Empty() {}

	typedef T type;
};


namespace detail {

template <typename T>
struct is_empty : std::false_type
{};

template <typename T>
struct is_empty<Empty<T>> : std::true_type
{};

} // namespace detail

/**
 * Get whether given type is an Empty type.
 * @tparam T Type to check
 */
template <typename T>
constexpr static bool is_empty_v = detail::is_empty<T>::value;


namespace detail {

template <typename T>
struct add_empty
{
	typedef Empty<T> type;
};

template <typename T>
struct add_empty<Empty<T>>
{
	typedef Empty<T> type;
};

} // namespace detail

/**
 * Add an Empty wrapper to given type if not already an Empty wrapper is given.
 * @tparam T Type to add Empty to
 */
template <typename T>
using add_empty_t = typename detail::add_empty<T>::type;


namespace detail {

template <typename T>
struct remove_empty
{
	typedef T type;
};

template <typename T>
struct remove_empty<Empty<T>>
{
	typedef T type;
};

} // namespace detail

/**
 * Remove Empty wrapper of given type if it is of form Empty<U>.
 * @tparam T Type to remove Empty from
 */
template <typename T>
using remove_empty_t = typename detail::remove_empty<T>::type;

} // namespace hate
