#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>
#include "hate/bitset.h"

namespace cereal {

template <typename Archive, size_t N, typename WordType>
void CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, hate::bitset<N, WordType>& bitset)
{
	auto bitset_words = bitset.to_array();
	ar(CEREAL_NVP(bitset_words));
	bitset = hate::bitset<N, WordType>(bitset_words);
}

} // namespace cereal
