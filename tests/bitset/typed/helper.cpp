#include <random>
#include <stddef.h>

size_t random_integer(size_t min, size_t max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> random(min, max);
	return random(gen);
}
