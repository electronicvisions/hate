#pragma once
#include "hate/bitset.h"
#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>

namespace cereal {

template <typename Archive, size_t N, typename WordType>
std::enable_if_t<N != 0> CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, hate::bitset<N, WordType>& bitset)
{
	auto bitset_words = bitset.to_array();
	ar(CEREAL_NVP(bitset_words));
	bitset = hate::bitset<N, WordType>(bitset_words);
}

template <typename Archive, size_t N, typename WordType>
std::enable_if_t<N == 0> CEREAL_SERIALIZE_FUNCTION_NAME(Archive&, hate::bitset<N, WordType>&)
{}

} // namespace cereal
