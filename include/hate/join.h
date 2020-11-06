#pragma once
#include <sstream>
#include <string>

namespace hate {

/**
 * Join an iterator range of entries with delimiter in-between.
 * @tparam OstreamT Sink providing ostream-operator interface
 * @tparam IteratorT Type of begin/end iterators
 * @tparam DelimiterT Type of delimiter
 * @param os OstreamT instance to be modified
 * @param begin Iterator pointing to begin
 * @param end Iterator pointing to end
 * @param delimiter Delimiter to insert between iterable elements
 */
template <typename OstreamT, typename IteratorT, typename DelimiterT>
void join(OstreamT& os, IteratorT const& begin, IteratorT const& end, DelimiterT const& delimiter)
{
	for (auto it = begin; it != end; ++it) {
		os << *it;
		if (auto tit = it; ++tit != end) {
			os << delimiter;
		}
	}
}

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
	join(os, iterable.begin(), iterable.end(), delimiter);
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

/**
 * Join iterable to string with delimiter in-between.
 * @tparam IteratorT Iterator type
 * @tparam DelimiterT Type of delimiter
 * @param begin Iterator pointing to begin
 * @param end Iterator pointing to end
 * @param delimiter Delimiter to use in-between
 */
template <typename IteratorT, typename DelimiterT>
std::string join_string(IteratorT const& begin, IteratorT const& end, DelimiterT const& delimiter)
{
	std::stringstream ss;
	join(ss, begin, end, delimiter);
	return ss.str();
}

} // namespace hate
