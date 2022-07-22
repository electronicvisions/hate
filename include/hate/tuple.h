#pragma once
#include "hate/nil.h"
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace hate {

namespace detail {

/**
 * Get whether given tuples have given size.
 * @tparam Size Size to check against
 * @tparam Head First tuple
 * @tparam Tuples Other tuples
 * @return Boolean value
 */
template <size_t Size, typename Head, typename... Tuples>
constexpr bool has_tuple_size()
{
	if constexpr (std::tuple_size_v<std::remove_reference_t<Head>> != Size) {
		return false;
	}
	if constexpr (sizeof...(Tuples) != 0) {
		return has_tuple_size<Size, Tuples...>();
	}
	return true;
}

/**
 * Get whether given tuples have equal size.
 * @tparam Head First tuple
 * @tparam Tuples Other tuples
 * @return Boolean value
 */
template <typename Head, typename... Tuples>
constexpr bool has_equal_tuple_size()
{
	return has_tuple_size<std::tuple_size_v<std::remove_reference_t<Head>>, Head, Tuples...>();
}

/**
 * Apply given function to arguments from tuples at given position.
 * @tparam Position Position to get tuple elements from
 * @tparam F Function to for_each
 * @tparam Tuples Tuples to get elements from
 * @return Result of function invokation
 */
template <size_t Position, typename F, typename... Tuples>
auto for_each(F&& f, Tuples&&... tuples)
{
	if constexpr (std::is_same_v<
	                  typename std::invoke_result<
	                      F, decltype(std::get<Position>(std::forward<Tuples>(tuples)))...>::type,
	                  void>) {
		std::invoke(std::forward<F>(f), std::get<Position>(std::forward<Tuples>(tuples))...);
		return Nil{};
	} else {
		return std::invoke(std::forward<F>(f), std::get<Position>(std::forward<Tuples>(tuples))...);
	}
}

/**
 * Apply given function to arguments from tuples in order of positions given by sequence.
 * @tparam Positions Positions to get tuple elements from
 * @tparam F Function to for_each
 * @tparam Tuples Tuples to get elements from
 * @return Tuple of results of function invokation at given positions
 */
template <size_t FirstPosition, size_t... Positions, typename F, typename... Tuples>
auto for_each(
    std::integer_sequence<size_t, FirstPosition, Positions...> const&, F&& f, Tuples&&... tuples)
{
	if constexpr (sizeof...(Positions) != 0) {
		auto first = std::make_tuple(for_each<FirstPosition, F, Tuples...>(
		    std::forward<F>(f), std::forward<Tuples>(tuples)...));
		return std::tuple_cat(
		    first, for_each(
		               std::integer_sequence<size_t, Positions...>{}, std::forward<F>(f),
		               std::forward<Tuples>(tuples)...));
	} else {
		return std::make_tuple(for_each<FirstPosition, F, Tuples...>(
		    std::forward<F>(f), std::forward<Tuples>(tuples)...));
	}
}

} // namespace detail

/**
 * Apply given function to arguments from tuples in order.
 * @tparam F Function to for_each
 * @tparam Head First tuple to get input from
 * @tparam Tuples Other tuples to get input from
 * @return Tuple of results of function invokation for all tuple elements
 */
template <typename F, typename Head, typename... Tuples>
auto for_each(F&& f, Head&& head, Tuples&&... tuples)
{
	static_assert(
	    detail::has_equal_tuple_size<Head, Tuples...>(),
	    "for_each requires all input tuples to be of same size.");
	return detail::for_each(
	    std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Head>>>(),
	    std::forward<F>(f), std::forward<Head>(head), std::forward<Tuples>(tuples)...);
}

} // namespace hate
