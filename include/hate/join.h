#pragma once
#include <sstream>
#include <string>

namespace hate {

/**
 * Join iterable entries with delimiter in-between.
 * @tparam OstreamT Sink providing ostream-operator interface
 * @tparam IterableT Type of iterable
 * @tparam DelimiterT Type of delimiter
 * @param os OstreamT instance to be modified
 * @param iterable Iterable to join
 * @param delimiter Delimiter to insert between iterable elements
 */
template <typename OstreamT, typename IterableT, typename DelimiterT>
void join(OstreamT& os, IterableT const& iterable, DelimiterT const& delimiter)
{
	for (auto it = iterable.cbegin(); it != iterable.cend(); ++it) {
		os << *it;
		if (it != iterable.cend() - 1) {
			os << delimiter;
		}
	}
}

/**
 * Join iterable to string with delimiter in-between.
 * @tparam IterableT Type of iterable
 * @tparam DelimiterT Type of delimiter
 * @param iterable Iterable instance to join
 * @param delimiter Delimiter to use in-between
 */
template <typename IterableT, typename DelimiterT>
std::string join_string(IterableT const& iterable, DelimiterT const& delimiter)
{
	std::stringstream ss;
	join(ss, iterable, delimiter);
	return ss.str();
}

} // namespace hate
