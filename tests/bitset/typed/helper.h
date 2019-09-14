#pragma once
#include <array>
#include <random>
#include <stddef.h>

/**
 * Fill array of integers with random values.
 * @tparam T Type of array entries
 * @tparam N Array-size
 */
template <class T, size_t N>
std::array<T, N> random_array()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<T> random;
	std::array<T, N> ret;
	for (auto& word : ret) {
		word = random(gen);
	}
	return ret;
}

size_t random_integer(size_t min, size_t max);
