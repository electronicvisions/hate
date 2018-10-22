#pragma once
#include "cereal/cereal.hpp"

#ifdef __cpp_lib_optional

#include <optional>

namespace hate {

template <typename T>
using optional = std::optional<T>;

using nullopt_t = std::nullopt_t;
inline constexpr std::nullopt_t const& nullopt = std::nullopt;

} // namespace hate

#else

#include <experimental/optional>

namespace hate {

template <typename T>
using optional = std::experimental::optional<T>;

using nullopt_t = std::experimental::nullopt_t;
inline constexpr std::experimental::nullopt_t const& nullopt = std::experimental::nullopt;

} // namespace hate

#endif

namespace cereal {

template <class Archive, typename T> inline
void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, hate::optional<T> const& optional)
{
	if(!optional) {
		ar(CEREAL_NVP_("nullopt", true));
	} else {
		ar(CEREAL_NVP_("nullopt", false),
		CEREAL_NVP_("data", *optional));
	}
}

template <class Archive, typename T> inline
void CEREAL_LOAD_FUNCTION_NAME(Archive& ar, hate::optional<T>& optional)
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
