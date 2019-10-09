#pragma once

namespace hate {

namespace detail {

template <typename T>
struct remove_all_qualifiers
{
	typedef T type;
};

template <typename T>
struct remove_all_qualifiers<T*>
{
	typedef typename remove_all_qualifiers<T>::type type;
};

template <typename T>
struct remove_all_qualifiers<T const>
{
	typedef typename remove_all_qualifiers<T>::type type;
};

template <typename T>
struct remove_all_qualifiers<T volatile>
{
	typedef typename remove_all_qualifiers<T>::type type;
};

template <typename T>
struct remove_all_qualifiers<T const volatile>
{
	typedef typename remove_all_qualifiers<T>::type type;
};

template <typename T>
struct remove_all_qualifiers<T&>
{
	typedef typename remove_all_qualifiers<T>::type type;
};

template <typename T>
struct remove_all_qualifiers<T&&>
{
	typedef typename remove_all_qualifiers<T>::type type;
};

} // namespace detail

/**
 * Remove arbitrary combinations of pointer, reference and const/volatile qualifiers from a type.
 * @tparam T Type to remove all qualifiers from
 */
template <typename T>
struct remove_all_qualifiers
{
	typedef typename detail::remove_all_qualifiers<T>::type type;
};

template <typename T>
using remove_all_qualifiers_t = typename remove_all_qualifiers<T>::type;

} // namespace hate
