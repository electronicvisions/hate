#pragma once
#include <tuple>
#include <typeinfo>

namespace hate {

/**
 * @file type_list.h
 * @brief This header contains helper classes for handling
 *        C++ (variadic) type lists.
 */

/**
 * @brief type_list contains a variadic type list.
 */
template <typename... Ts>
struct type_list
{};


/**
 * @brief convert other types to typelist.
 */
template <typename FromT>
struct type_list_from
{
	static_assert(sizeof(FromT) == 0, "Conversion for type not supported.");
};

template <typename... Ts>
using type_list_from_t = typename type_list_from<Ts...>::type;

template <template <typename...> class FromT, typename... Ts>
struct type_list_from<FromT<Ts...>>
{
	using type = type_list<Ts...>;
};

/**
 * @brief convert type_list to any variadic data-type
 */
template <template <typename...> class, typename TL>
struct type_list_to
{
	static_assert(sizeof(TL) == 0, "Conversion only supported for type lists.");
};

template <template <typename...> class TargetT, typename TL>
using type_list_to_t = typename type_list_to<TargetT, TL>::type;

template <template <typename...> class TargetT, typename... Ts>
struct type_list_to<TargetT, type_list<Ts...>>
{
	using type = TargetT<Ts...>;
};


/**
 * @brief The corresponding type_list trait.
 */
template <typename T>
struct is_type_list : std::false_type
{};

template <typename... Ts>
struct is_type_list<type_list<Ts...>> : std::true_type
{};


namespace detail {
template <typename Ta, typename Tb>
struct concat;

template <typename... Ta, typename... Tb>
struct concat<type_list<Ta...>, type_list<Tb...>>
{
	using type = type_list<Ta..., Tb...>;
};
} // namespace detail

/**
 * @brief Concatenation of the types of two type_list types into a new
 * type_list (left to right).
 */
template <typename... Ts>
using concat_t = typename detail::concat<Ts...>::type;


namespace detail {
template <typename... TLs>
struct multi_concat;

template <typename FirstTL, typename... TLs>
struct multi_concat<FirstTL, TLs...>
{
	using type = typename multi_concat<FirstTL, typename multi_concat<TLs...>::type>::type;
};

template <typename One, typename Two>
struct multi_concat<One, Two>
{
	using type = concat_t<One, Two>;
};

template <typename LastTL>
struct multi_concat<LastTL>
{
	static_assert(is_type_list<LastTL>::value);
	using type = LastTL;
};

template <typename T>
struct to_false_type : std::false_type
{};

template <typename... Ts>
struct is_in_type_list : std::false_type
{};

template <typename T, typename U, typename... Us>
struct is_in_type_list<T, U, Us...>
{
	static constexpr const bool value =
	    std::is_same<T, U>::value || is_in_type_list<T, Us...>::value;
};

/**
 * @brief Check if type is in type_list.
 */
template <typename T, typename... Us>
struct is_in_type_list<T, type_list<Us...>>
{
	static constexpr bool value = detail::is_in_type_list<T, Us...>::value;
};

} // namespace detail

template <typename... Ts>
using is_in_type_list = detail::is_in_type_list<Ts...>;

/**
 * @brief Concatenation of N type_list types into a single type_list.
 */
template <typename... Ts>
using multi_concat_t = typename detail::multi_concat<Ts...>::type;


/**
 * @brief Calculate the index of a type in a type_list.
 * @return The 0-based index of the type.
 * @note Duplicates and non-existing type yield static_asserts.
 */
template <typename T, typename TL>
struct index_type_list_by_type
{
	static_assert(detail::to_false_type<T>::value, "Cannot find the type in the type_list");
};

template <typename T, typename... Ts>
struct index_type_list_by_type<T, type_list<T, Ts...>>
{
	static constexpr size_t value = 0;
	static_assert((!detail::is_in_type_list<T, Ts...>::value), "Duplicate type in type_list");
};

template <typename T, typename U, typename... Ts>
struct index_type_list_by_type<T, type_list<U, Ts...>>
{
	static constexpr size_t value = 1 + index_type_list_by_type<T, type_list<Ts...>>::value;
};


/**
 * @brief Calculate the type of an index in a type_list.
 * @return The type of a 0-based index.
 * @note Bounds checking is performed.
 */
template <size_t I, typename TL>
struct index_type_list_by_integer
{
	static_assert(detail::to_false_type<TL>::value, "Parameter not a type_list");
};

template <size_t I, typename... Ts>
struct index_type_list_by_integer<I, type_list<Ts...>>
{
	static_assert(I < sizeof...(Ts), "Index is out of bounds");
	using type = typename std::tuple_element<I, std::tuple<Ts...>>::type;
};

/**
 * @brief Calculate the size of a type_list.
 * @return Number of entries in type_list.
 * @note Duplicate types are counted.
 */
template <typename TL>
struct type_list_size
{
	static_assert(detail::to_false_type<TL>::value, "Parameter not a type_list");
};

template <typename... Ts>
struct type_list_size<type_list<Ts...>>
{
	static constexpr size_t value = sizeof...(Ts);
};

/**
 * Apply given filter to type_list.
 * @tparam Filter Filter generating boolean value for each type in type_list
 * @tparam TL type_list to filter
 */
template <template <typename> typename Filter, typename TL>
struct filter_type_list;

template <template <typename> typename Filter, typename T, typename... Ts>
struct filter_type_list<Filter, hate::type_list<T, Ts...>>
{
	typedef std::conditional_t<
	    Filter<T>::value,
	    hate::multi_concat_t<
	        hate::type_list<T>,
	        typename filter_type_list<Filter, hate::type_list<Ts...>>::type>,
	    typename filter_type_list<Filter, hate::type_list<Ts...>>::type>
	    type;
};

template <template <typename> typename Filter>
struct filter_type_list<Filter, hate::type_list<>>
{
	typedef hate::type_list<> type;
};

template <template <typename> typename Filter, typename TL>
using filter_type_list_t = typename filter_type_list<Filter, TL>::type;

/**
 * Apply type intersection to type_list pair.
 * @tparam TL1 first type_list
 * @tparam TL2 second type_list
 */
template <typename TL1, typename TL2>
struct type_list_intersection
{
	template <typename T>
	using filter = is_in_type_list<T, TL2>;

	typedef filter_type_list_t<filter, TL1> type;
};

template <typename TL1, typename TL2>
using type_list_intersection_t = typename type_list_intersection<TL1, TL2>::type;

namespace detail {

template <typename T, typename... Ts>
struct type_list_make_unique
{
	typedef hate::multi_concat_t<
	    std::conditional_t<
	        hate::is_in_type_list<T, hate::type_list<Ts...>>::value,
	        hate::type_list<>,
	        hate::type_list<T>>,
	    typename type_list_make_unique<Ts...>::type>
	    type;
};

template <typename T>
struct type_list_make_unique<T>
{
	typedef hate::type_list<T> type;
};

} // namespace detail

/**
 * Reduce type list to unique types.
 * @param TL Type list to reduce
 */
template <typename TL>
struct type_list_unique;

template <typename... Ts>
struct type_list_unique<hate::type_list<Ts...>>
{
	typedef typename detail::type_list_make_unique<Ts...>::type type;
};

template <>
struct type_list_unique<hate::type_list<>>
{
	typedef typename hate::type_list<> type;
};

template <typename TL>
using type_list_unique_t = typename type_list_unique<TL>::type;

} // namespace hate
