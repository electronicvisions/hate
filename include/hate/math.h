#pragma once
#include <stddef.h>

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

} // namespace math
} // namespace hate
