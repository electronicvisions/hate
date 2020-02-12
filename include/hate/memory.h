#pragma once

#include <memory>
#include <type_traits>
#include <utility>


namespace hate {
namespace memory {

template <typename T, typename Tuple>
std::unique_ptr<T> make_unique_from_tuple(Tuple&& t);

namespace detail {
template <typename T, typename Tuple, size_t... I>
std::unique_ptr<T> make_unique_from_tuple_impl(Tuple&& t, std::index_sequence<I...>)
{
	return std::make_unique<T>(std::get<I>(std::forward<Tuple>(t))...);
}
}

template <typename T, typename Tuple>
std::unique_ptr<T> make_unique_from_tuple(Tuple&& t)
{
	return detail::make_unique_from_tuple_impl<T>(
	    std::forward<Tuple>(t),
	    std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}
}
}
