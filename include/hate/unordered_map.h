#pragma once

#include <functional>
#include <optional>
#include <unordered_map>

namespace hate {

/**
 * Unified getter for unordered_map that returns an optional.
 *
 * @param map Map from which to get value.
 * @param key Key for value.
 * @return Optional that contains reference to value if contained in map, nullopt otherwise.
 */
template <typename MapT>
std::optional<std::reference_wrapper<typename MapT::mapped_type>> get(
    MapT& map, typename MapT::key_type const& key)
{
	if (map.contains(key)) {
		return std::make_optional(std::ref(map.at(key)));
	} else {
		return std::nullopt;
	}
}

/**
 * Unified getter for unordered_map that returns an optional. Constant variant.
 *
 * @param map Map from which to get value.
 * @param key Key for value.
 * @return Optional that contains const reference to value if contained in map, nullopt otherwise.
 */
template <typename MapT>
std::optional<std::reference_wrapper<typename MapT::mapped_type const>> cget(
    MapT const& map, typename MapT::key_type const& key)
{
	if (map.contains(key)) {
		return std::make_optional(std::cref(map.at(key)));
	} else {
		return std::nullopt;
	}
}

} // namespace hate
