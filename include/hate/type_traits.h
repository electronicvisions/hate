#pragma once

#include <type_traits>

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

/**
 * Helpers for is_detected, detected_t and detected_or (will be provided by TS2
 * in the future)
 */
template<typename Default, typename AlwaysVoid, template<typename...> typename Op, typename... Args>
struct detector {
	using value_t = std::false_type;
	using type = Default;
};

template <typename Default, template<typename...> typename Op, typename... Args>
struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
	using value_t = std::true_type;
	using type = Op<Args...>;
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

/**
 * nonesuch cannot be constructed, destroyed, or copied. (future TS2)
 */
struct nonesuch {
	nonesuch() = delete;
	~nonesuch() = delete;
	nonesuch(nonesuch const&) = delete;
	void operator=(nonesuch const&) = delete;
};

/**
 * detection types which will be provided by TS2
 */
template <template<class...> class Op, class... Args>
using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

template <template<class...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

template <class Default, template<class...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;

template< template<class...> class Op, class... Args >
constexpr bool is_detected_v = is_detected<Op, Args...>::value;

template< class Default, template<class...> class Op, class... Args >
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

template <class Expected, template<class...> class Op, class... Args>
using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

template <class Expected, template<class...> class Op, class... Args>
constexpr bool is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;

template <class To, template<class...> class Op, class... Args>
using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;

template <class To, template<class...> class Op, class... Args>
constexpr bool is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;


/**
 * Check if a type adheres to the Container concept.
 * @tparam T Type to check
 */
template <typename T, typename = void>
struct is_container : std::false_type
{};

template <typename T>
struct is_container<
    T,
    std::void_t<
        typename T::value_type,
        typename T::size_type,
        typename T::allocator_type,
        typename T::iterator,
        typename T::const_iterator,
        decltype(std::declval<T>().size()),
        decltype(std::declval<T>().begin()),
        decltype(std::declval<T>().end()),
        decltype(std::declval<T>().cbegin()),
        decltype(std::declval<T>().cend())>> : public std::true_type
{};

template <typename T>
constexpr static bool is_container_v = is_container<T>::value;

} // namespace hate
