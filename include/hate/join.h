#pragma once
#include <iosfwd>
#include <sstream>
#include <type_traits>
#include <utility>

namespace hate {

/**
 * Object which is serializable into an ostream by joining an iterator range of entries with
 * delimiter in-between.
 * @tparam IteratorT Type of begin/end iterators
 * @tparam DelimiterT Type of delimiter
 * @param begin Iterator pointing to begin
 * @param end Iterator pointing to end
 * @param delimiter Delimiter to insert between iterable elements
 */
template <typename IteratorT, typename DelimiterT>
struct Join
{
	IteratorT begin;
	IteratorT end;
	DelimiterT delimiter;
};

template <typename IteratorT, typename DelimiterT>
std::ostream& operator<<(std::ostream& os, Join<IteratorT, DelimiterT> const& value)
{
	for (auto it = value.begin; it != value.end; ++it) {
		os << *it;
		if (auto tit = it; ++tit != value.end) {
			os << value.delimiter;
		}
	}
	return os;
}


/**
 * Object which is serializable into an ostream by joining an iterator range of entries with
 * delimiter in-between. A transformation is applied to each entry prior to serialization.
 * @tparam IteratorT Type of begin/end iterators
 * @tparam DelimiterT Type of delimiter
 * @tparam FunctorT Type of transformation, is required to be callable with *IteratorT as argument
 * type
 * @param begin Iterator pointing to begin
 * @param end Iterator pointing to end
 * @param delimiter Delimiter to insert between iterable elements
 * @param functor Transformation to apply to each element of iterator range
 */
template <typename IteratorT, typename DelimiterT, typename FunctorT>
struct JoinTransformed
{
	IteratorT begin;
	IteratorT end;
	DelimiterT delimiter;
	FunctorT functor;
};

template <typename IteratorT, typename DelimiterT, typename FunctorT>
std::ostream& operator<<(
    std::ostream& os, JoinTransformed<IteratorT, DelimiterT, FunctorT> const& value)
{
	for (auto it = value.begin; it != value.end; ++it) {
		os << value.functor(*it);
		if (auto tit = it; ++tit != value.end) {
			os << value.delimiter;
		}
	}
	return os;
}


/**
 * Create object which is serializable by joining an iterator range of entries with delimiter
 * in-between.
 * @tparam IteratorT Type of begin/end iterators
 * @tparam DelimiterT Type of delimiter
 * @param begin Iterator pointing to begin
 * @param end Iterator pointing to end
 * @param delimiter Delimiter to insert between iterable elements
 */
template <typename IteratorT, typename DelimiterT>
Join<IteratorT, DelimiterT> join(IteratorT&& begin, IteratorT&& end, DelimiterT&& delimiter)
{
	return Join<IteratorT, DelimiterT>{
	    std::forward<IteratorT>(begin), std::forward<IteratorT>(end),
	    std::forward<DelimiterT>(delimiter)};
}


/**
 * Create object which is serializable by joining an iterable with delimiter in-between.
 * @tparam IterableT Type of iterable
 * @tparam DelimiterT Type of delimiter
 * @param iterable Iterable to join
 * @param delimiter Delimiter to insert between iterable elements
 */
template <typename IterableT, typename DelimiterT>
auto join(IterableT&& iterable, DelimiterT&& delimiter)
{
	return join(iterable.begin(), iterable.end(), std::forward<DelimiterT>(delimiter));
}


/**
 * Create object which is serializable by joining an iterator range of entries with delimiter
 * in-between and applying transformation prior to serialization.
 * @tparam IteratorT Type of begin/end iterators
 * @tparam DelimiterT Type of delimiter
 * @tparam FunctorT Type of transformation, is required to be callable with *IteratorT as argument
 * type
 * @param begin Iterator pointing to begin
 * @param end Iterator pointing to end
 * @param delimiter Delimiter to insert between iterable elements
 * @param functor Transformation to apply to each element of iterator range
 */
template <typename IteratorT, typename DelimiterT, typename FunctorT>
JoinTransformed<IteratorT, DelimiterT, FunctorT> join(
    IteratorT&& begin, IteratorT&& end, DelimiterT&& delimiter, FunctorT&& functor)
{
	return JoinTransformed<IteratorT, DelimiterT, FunctorT>{
	    std::forward<IteratorT>(begin), std::forward<IteratorT>(end),
	    std::forward<DelimiterT>(delimiter), std::forward<FunctorT>(functor)};
}


/**
 * Create object which is serializable by joining an iterable of entries with delimiter
 * in-between and applying transformation prior to serialization.
 * @tparam IterableT Type of iterable
 * @tparam DelimiterT Type of delimiter
 * @tparam FunctorT Type of transformationm, is required to be callable with any element of
 * IterableT
 * @param iterable Iterable to join
 * @param delimiter Delimiter to insert between iterable elements
 * @param functor Transformation to apply to each element of iterator range
 */
template <typename IterableT, typename DelimiterT, typename FunctorT>
auto join(IterableT&& iterable, DelimiterT&& delimiter, FunctorT&& functor)
{
	return join(
	    iterable.begin(), iterable.end(), std::forward<DelimiterT>(delimiter),
	    std::forward<FunctorT>(functor));
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
	ss << join(iterable, delimiter);
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
	ss << join(begin, end, delimiter);
	return ss.str();
}

} // namespace hate
