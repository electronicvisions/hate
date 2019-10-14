#pragma once
#include <cereal/cereal.hpp>

#include "hate/optional.h"

namespace cereal {

template <class Archive, typename T>
inline void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, hate::optional<T> const& optional)
{
	if (!optional) {
		ar(CEREAL_NVP_("nullopt", true));
	} else {
		ar(CEREAL_NVP_("nullopt", false), CEREAL_NVP_("data", *optional));
	}
}

template <class Archive, typename T>
inline void CEREAL_LOAD_FUNCTION_NAME(Archive& ar, hate::optional<T>& optional)
{
	bool nullopt;
	ar(CEREAL_NVP_("nullopt", nullopt));
	if (nullopt) {
		optional = hate::nullopt;
	} else {
		T value;
		ar(CEREAL_NVP_("data", value));
		optional = std::move(value);
	}
}

} // namespace cereal
