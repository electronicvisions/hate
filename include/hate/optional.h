#pragma once

#ifdef __cpp_lib_optional
#include <optional>
#else
#include <experimental/optional>
#endif

namespace hate {

#ifdef __cpp_lib_optional

template <typename T>
using optional = std::optional<T>;

using nullopt_t = std::nullopt_t;
inline constexpr std::nullopt_t const& nullopt = std::nullopt;

#else

template <typename T>
using optional = std::experimental::optional<T>;

using nullopt_t = std::experimental::nullopt_t;
inline constexpr std::experimental::nullopt_t const& nullopt = std::experimental::nullopt;

#endif

/**
 * Checks the equality of two variables of optional type.
 * If both variables hold values the function returns the boolean equality comparison
 * of the two values.
 * In case of at least one the values is absent the fall-back-value is returned.
 *
 * @param a First value to be compared.
 * @param b Second value do be compared.
 * @param fall_back Boolean value to be returned if a or b is uninitialized.
 *
 * @return Equality comparison of a and b if both hold values, fall-back-value otherwise.
 */
template <template <typename> typename Optional, typename T1, typename T2>
bool compare_optional_equal(Optional<T1> a, Optional<T2> b, bool const fall_back)
{
	// check if both values are defined
	bool const have_values = (static_cast<bool>(a) && static_cast<bool>(b));
	// return boolean comparison if both are defined and fall_back otherwise
	return have_values ? (*a == *b) : fall_back;
}

} // namespace hate
