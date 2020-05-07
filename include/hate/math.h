#pragma once
#include <cstddef>

namespace hate {
namespace math {

/**
 * @brief Calculates the minimal number of bits needed to store the integer.
 * @param n Integer number
 * @return Result of calculation
 */
constexpr size_t num_bits(size_t n)
{
	return ((n < 2) ? 1 : (1 + num_bits(n / 2)));
}

/**
 * @brief Calculates the integer division rounded up.
 * @param number Number to calculate division for
 * @param divisor Divisor
 * @return Result of division
 */
constexpr size_t round_up_integer_division(size_t number, size_t divisor)
{
	return (number + divisor - 1) / divisor;
}

/**
 * @brief Rounds up a number to full multiples.
 * @param number Number to round up
 * @param multiple Multiple to use as base for rounding
 * @return Rounded number
 */
constexpr size_t round_up_to_multiple(size_t number, size_t multiple)
{
	return round_up_integer_division(number, multiple) * multiple;
}

/**
 * Calculate the power of an integer value.
 * @param base Base value
 * @param exp Exponent value
 */
constexpr size_t pow(size_t base, size_t exp)
{
	return (exp != 0) ? (base * pow(base, exp - 1)) : 1;
}


/**
 * Calculate the logarithm to the power of two of an integer value.
 * @param value Value to calculate log2 for
 */
constexpr size_t log2(size_t value)
{
	return (value > 1) ? 1 + log2(value >> 1) : 0;
}

} // namespace math
} // namespace hate
