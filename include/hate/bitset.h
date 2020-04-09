#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cstddef>
#include <ostream>
#include <sstream>

namespace hate {

namespace detail {

/**
 * Recursively set an index-range of bits to true.
 * @tparam T Return type of bitmask
 * @param cur Current bitmask
 * @param max Maximal index to set to true
 * @param pos Current index position
 */
template <class T>
constexpr T recurse_fill_bits(T cur, size_t max, size_t pos)
{
	return (pos == max) ? cur : recurse_fill_bits(cur | static_cast<T>(1) << pos, max, pos + 1);
}

} // namespace detail

/**
 * Create bitmask with bits set to true up to (including) a specified index.
 * @tparam T Type of bitmask
 * @tparam max_index Index up to which to set bits to true
 */
template <class T>
constexpr T fill_bits(size_t max_index)
{
	return detail::recurse_fill_bits(static_cast<T>(0), max_index, 0);
}


/**
 * Constant-size bitset. The bitset is accessible as array of unsigned integer words with
 * configurable word-size. Logical operations are implemented for smaller-equal sized right-hand
 * side bitsets. All logical operations are constexpr evaluable. The interface of std::bitset is
 * to be mimicked for equal-sized operations as well as conversion to std::string and unsigned
 * integer.
 * @tparam N Number of bits
 * @tparam WordType Type of underlying word storage
 */
template <size_t N, class WordType = unsigned long>
class bitset
{
	static_assert(std::is_integral<WordType>::value, "bitset only works with integral word type.");
	static_assert(std::is_unsigned<WordType>::value, "bitset only works with unsigned word type.");

public:
	typedef WordType word_type;
	constexpr static size_t num_bits_per_word = sizeof(word_type) * CHAR_BIT;
	constexpr static size_t num_words = (N + num_bits_per_word - 1) / num_bits_per_word;
	constexpr static size_t size = N;

	/**
	 * Default construct bitset with all bits disabled.
	 */
	constexpr bitset();

	/**
	 * Construct bitset from an unsigned integer.
	 * @param value Value to construct from
	 */
	constexpr bitset(uintmax_t const& value);

	/**
	 * Construct bitset from array of words.
	 * @param value Value to construct from
	 */
	template <size_t M>
	constexpr bitset(std::array<word_type, M> const& value);

	/**
	 * Construct bitset from possibly differently sized bitset.
	 * Not present bits in the other bitset will be set to false (M < N), bits not present in this
	 * instance are ignored (M > N).
	 * @tparam M Size of bitset to construct instance from
	 * @tparam OtherWordType Type of underlying array of other bitset
	 * @param other Bitset to construct instance from
	 */
	template <size_t M, class OtherWordType>
	constexpr bitset(bitset<M, OtherWordType> const& other);

	/**
	 * Assign bitset from possibly differently sized bitset.
	 * Not present bits in the other bitset will be set to false (M < N), bits not present in this
	 * instance are ignored (M > N).
	 * @tparam M Size of bitset to assign to instance
	 * @tparam OtherWordType Type of underlying array of other bitset
	 * @param other Bitset to assign to instance
	 */
	template <size_t M, class OtherWordType>
	constexpr bitset& operator=(bitset<M, OtherWordType> const& other);

	/**
	 * Perform &= operation on bitset with smaller or equal sized other bitset.
	 * @tparam M Size of other bitset
	 * @param other Other bitset
	 * @return (instance &= other)
	 */
	template <size_t M>
	constexpr bitset& operator&=(bitset<M, word_type> const& other);

	/**
	 * Perform |= operation on bitset with smaller or equal sized other bitset.
	 * @tparam M Size of other bitset
	 * @param other Other bitset
	 * @return (instance |= other)
	 */
	template <size_t M>
	constexpr bitset& operator|=(bitset<M, word_type> const& other);

	/**
	 * Perform ^= operation on bitset with smaller or equal sized other bitset.
	 * @tparam M Size of other bitset
	 * @param other Other bitset
	 * @return (instance ^= other)
	 */
	template <size_t M>
	constexpr bitset& operator^=(bitset<M, word_type> const& other);

	/**
	 * Perform & operation on bitset with smaller or equal sized other bitset.
	 * @tparam M Size of other bitset
	 * @param other Other bitset
	 * @return (instance & other)
	 */
	template <size_t M>
	constexpr bitset operator&(bitset<M, word_type> const& other) const;

	/**
	 * Perform | operation on bitset with smaller or equal sized other bitset.
	 * @tparam M Size of other bitset
	 * @param other Other bitset
	 * @return (instance | other)
	 */
	template <size_t M>
	constexpr bitset operator|(bitset<M, word_type> const& other) const;

	/**
	 * Perform ^ operation on bitset with smaller or equal sized other bitset.
	 * @tparam M Size of other bitset
	 * @param other Other bitset
	 * @return (instance ^ other)
	 */
	template <size_t M>
	constexpr bitset operator^(bitset<M, word_type> const& other) const;

	/**
	 * Perform >>= operation on bitset.
	 * @param shift Number of bits to shift right
	 * @return (instance >>= shift)
	 */
	constexpr bitset& operator>>=(size_t shift);

	/**
	 * Perform <<= operation on bitset.
	 * @param shift Number of bits to shift right
	 * @return (instance <<= shift)
	 */
	constexpr bitset& operator<<=(size_t shift);

	/**
	 * Perform >> operation on bitset.
	 * @param shift Number of bits to shift right
	 * @return (instance >> shift)
	 */
	constexpr bitset operator>>(size_t shift) const;

	/**
	 * Perform << operation on bitset.
	 * @param shift Number of bits to shift right
	 * @return (instance << shift)
	 */
	constexpr bitset operator<<(size_t shift) const;

	/**
	 * Perform >>= operation on bitset for multiples of num_bits_per_word.
	 * @param word_shift Number of words to shift
	 * @return (instance >>= word_shift * num_bits_per_word)
	 */
	constexpr bitset& shift_words_right(size_t word_shift);

	/**
	 * Perform <<= operation on bitset for multiples of num_bits_per_word.
	 * @param word_shift Number of words to shift
	 * @return (instance <<= word_shift * num_bits_per_word)
	 */
	constexpr bitset& shift_words_left(size_t word_shift);

	/**
	 * Create new instance with flipped bits.
	 * @return New instance with all bits flipped.
	 */
	constexpr bitset operator~() const;

	/**
	 * Equality operator.
	 * @param other Bitset to compare to
	 * @return Result of comparison
	 */
	constexpr bool operator==(bitset const& other) const;

	/**
	 * Inequality operator.
	 * @param other Bitset to compare to
	 * @return Result of comparison
	 */
	constexpr bool operator!=(bitset const& other) const;

	/**
	 * Set all bits of bitset to true.
	 * @return Instance with all bits set to true.
	 */
	constexpr bitset& set();

	/**
	 * Set bit of bitset to boolean value.
	 * @param index Index of bit to set
	 * @param value Boolean value to set bit to
	 * @return Instance with bit set to value.
	 */
	constexpr bitset& set(size_t index, bool value = true);

	/**
	 * Set all bits of bitset to false.
	 * @return Instance with all bits set to false.
	 */
	constexpr bitset& reset();

	/**
	 * Set bit of bitset to false.
	 * @param index Index of bit to set
	 * @return Instance with bit set to false.
	 */
	constexpr bitset& reset(size_t index);

	/**
	 * Flip all bits.
	 * @return Instance with all bits flipped.
	 */
	constexpr bitset& flip();

	/**
	 * Flip bit.
	 * @param index Index of bit to flip
	 * @return Instance with bit flipped.
	 */
	constexpr bitset& flip(size_t index);

	/**
	 * Get boolean value of bit.
	 * @param Index of bit to get
	 * @return Boolean value of bit
	 */
	constexpr bool test(size_t index) const;

	/**
	 * Check if all bits are set to true.
	 * @return Result of check
	 */
	constexpr bool all() const;

	/**
	 * Check if any bit is set to true.
	 * @return Result of check
	 */
	constexpr bool any() const;

	/**
	 * Check if no bit is set to true.
	 * @return Result of check
	 */
	constexpr bool none() const;

	/**
	 * Get underlying array of words.
	 * @return Array of words
	 */
	constexpr std::array<word_type, num_words> const& to_array() const;

	/**
	 * Conversion operator to underlying array of words.
	 * @return Array of words
	 */
	constexpr explicit operator std::array<word_type, num_words> const&() const;

	/**
	 * Convert bitset to string. '1' represents a bit set to true, '0' represents a bit set to
	 * false.
	 * @return Bitset value as string
	 */
	std::string to_string() const;

	/**
	 * Conversion operator to string. '1' represents a bit set to true, '0' represents a bit set to
	 * false.
	 * @return Bitset value as string
	 */
	explicit operator std::string() const;

	/**
	 * Convert bitset to maximal size unsigned integer. If the bitset does not fit the integer, high
	 * bits are dropped.
	 * @return Bitset value as maximal-sized unsigned integer
	 */
	constexpr uintmax_t to_uintmax() const;

	/**
	 * Conversion operator to unsigned integer. If the bitset does not fit the integer,
	 * high bits are dropped.
	 * @return Bitset value as unsigned integer
	 */
	template <
	    typename UIntegerType,
	    typename = std::enable_if_t<std::is_unsigned<UIntegerType>::value>>
	constexpr explicit operator UIntegerType() const;

	/**
	 * Convert bitset to unsigned long integer. If the bitset does not fit the integer, high
	 * bits are dropped.
	 * @return Bitset value as unsigned long integer
	 */
	constexpr unsigned long to_ulong() const;

	/**
	 * Convert bitset to unsigned long long integer. If the bitset does not fit the integer, high
	 * bits are dropped.
	 * @return Bitset value as unsigned long long integer
	 */
	constexpr unsigned long long to_ullong() const;

private:
	constexpr word_type highest_word_bitmask() const;

	constexpr void sanitize();

	template <size_t M, class OtherWordType>
	friend class bitset;

	std::array<word_type, num_words> m_words;
};


template <size_t N, class WordType>
constexpr bitset<N, WordType>::bitset() : m_words()
{
	for (auto& word : m_words) {
		word = static_cast<word_type>(0);
	}
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>::bitset(uintmax_t const& value) : m_words()
{
	static_assert(
	    sizeof(uintmax_t) % sizeof(word_type) == 0, "uintmax not divisible by word_type.");

	if constexpr (N > 0) {
		constexpr size_t num_set_words = std::min(num_words, sizeof(uintmax_t) / sizeof(word_type));
		for (size_t i = 0; i < num_set_words; ++i) {
			m_words[i] = static_cast<word_type>(value >> (num_bits_per_word * i));
		}
		for (size_t i = num_set_words; i < num_words; ++i) {
			m_words[i] = static_cast<word_type>(0);
		}
		sanitize();
	}
}

template <size_t N, class WordType>
template <size_t M, class OtherWordType>
constexpr bitset<N, WordType>::bitset(bitset<M, OtherWordType> const& other) : m_words()
{
	static_assert(
	    std::max(sizeof(WordType), sizeof(OtherWordType)) %
	            std::min(sizeof(WordType), sizeof(OtherWordType)) ==
	        0,
	    "Word types must have a common divisor");

	if constexpr (sizeof(WordType) > sizeof(OtherWordType)) {
		for (auto& word : m_words) {
			word = static_cast<word_type>(0);
		}
		constexpr size_t word_size_factor = sizeof(WordType) / sizeof(OtherWordType);
		if constexpr (N >= M) {
			for (size_t i = 0; i < other.num_words; ++i) {
				m_words[i / word_size_factor] |=
				    (static_cast<WordType>(other.m_words[i])
				     << (other.num_bits_per_word * (i % word_size_factor)));
			}
		} else {
			for (size_t i = 0; i < std::min(other.num_words, word_size_factor * num_words); ++i) {
				m_words[i / word_size_factor] |=
				    (static_cast<WordType>(other.m_words[i])
				     << (other.num_bits_per_word * (i % word_size_factor)));
			}
			sanitize();
		}
	} else if constexpr (sizeof(WordType) == sizeof(OtherWordType)) {
		if constexpr (N >= M) {
			for (size_t i = 0; i < other.num_words; ++i) {
				m_words[i] = other.m_words[i];
			}
			for (size_t i = other.num_words; i < num_words; ++i) {
				m_words[i] = static_cast<word_type>(0);
			}
		} else {
			for (size_t i = 0; i < num_words; ++i) {
				m_words[i] = other.m_words[i];
			}
			sanitize();
		}
	} else {
		constexpr size_t word_size_factor = sizeof(OtherWordType) / sizeof(WordType);
		if constexpr (N >= M) {
			constexpr size_t num_set_words =
			    std::min(num_words, (other.num_words * word_size_factor));
			for (size_t i = 0; i < num_set_words; ++i) {
				m_words[i] = static_cast<WordType>(
				    other.m_words[i / word_size_factor] >>
				    (i % word_size_factor) * num_bits_per_word);
			}
			for (size_t i = num_set_words; i < num_words; ++i) {
				m_words[i] = static_cast<word_type>(0);
			}
		} else {
			for (size_t i = 0; i < num_words; ++i) {
				m_words[i] = static_cast<WordType>(
				    other.m_words[i / word_size_factor] >>
				    (i % word_size_factor) * num_bits_per_word);
			}
			sanitize();
		}
	}
}

template <size_t N, class WordType>
template <size_t M>
constexpr bitset<N, WordType>::bitset(std::array<WordType, M> const& value) : m_words()
{
	if constexpr (num_words > M) {
		for (size_t i = 0; i < M; ++i) {
			m_words[i] = value[i];
		}
		for (size_t i = M; i < num_words; ++i) {
			m_words[i] = static_cast<word_type>(0);
		}
	} else {
		for (size_t i = 0; i < num_words; ++i) {
			m_words[i] = value[i];
		}
		sanitize();
	}
}

template <size_t N, class WordType>
template <size_t M, class OtherWordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::operator=(bitset<M, OtherWordType> const& other)
{
	bitset<N, WordType> converter(other);
	m_words = converter.m_words;
	return *this;
}

template <size_t N, class WordType>
template <size_t M>
constexpr bitset<N, WordType>& bitset<N, WordType>::operator&=(bitset<M, WordType> const& other)
{
	static_assert(M <= N, "operator&= can't enlarge bitset");

	for (size_t i = 0; i < other.num_words; ++i) {
		m_words[i] &= other.m_words[i];
	}
	for (size_t i = other.num_words; i < num_words; ++i) {
		m_words[i] = static_cast<word_type>(0);
	}
	return *this;
}

template <size_t N, class WordType>
template <size_t M>
constexpr bitset<N, WordType>& bitset<N, WordType>::operator|=(bitset<M, WordType> const& other)
{
	static_assert(M <= N, "operator|= can't enlarge bitset");

	for (size_t i = 0; i < other.num_words; ++i) {
		m_words[i] |= other.m_words[i];
	}
	return *this;
}

template <size_t N, class WordType>
template <size_t M>
constexpr bitset<N, WordType>& bitset<N, WordType>::operator^=(bitset<M, WordType> const& other)
{
	static_assert(M <= N, "operator^= can't enlarge bitset");

	for (size_t i = 0; i < other.num_words; ++i) {
		m_words[i] ^= other.m_words[i];
	}
	return *this;
}

template <size_t N, class WordType>
template <size_t M>
constexpr bitset<N, WordType> bitset<N, WordType>::operator&(bitset<M, WordType> const& other) const
{
	bitset ret(*this);
	return (ret &= other);
}

template <size_t N, class WordType>
template <size_t M>
constexpr bitset<N, WordType> bitset<N, WordType>::operator|(bitset<M, WordType> const& other) const
{
	bitset ret(*this);
	return (ret |= other);
}

template <size_t N, class WordType>
template <size_t M>
constexpr bitset<N, WordType> bitset<N, WordType>::operator^(bitset<M, WordType> const& other) const
{
	bitset ret(*this);
	return (ret ^= other);
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::operator>>=(size_t const shift)
{
	if constexpr (N == 0) {
		return *this;
	}
	if (shift == 0) {
		return *this;
	}
	if (shift < size) {
		size_t const word_shift = shift / num_bits_per_word;
		size_t const offset = shift % num_bits_per_word;
		size_t const limit = num_words - word_shift - 1;
		if (offset == 0) {
			for (size_t i = 0; i <= limit; ++i) {
				m_words[i] = m_words[i + word_shift];
			}
		} else {
			size_t const sub_offset = num_bits_per_word - offset;
			for (size_t i = 0; i < limit; ++i) {
				m_words[i] =
				    ((m_words[i + word_shift] >> offset) |
				     (m_words[i + word_shift + 1] << sub_offset));
			}
			m_words[limit] = m_words[num_words - 1] >> offset;
		}
		for (size_t i = limit + 1; i < num_words; ++i) {
			m_words[i] = static_cast<word_type>(0);
		}
		return *this;
	} else {
		reset();
		return *this;
	}
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::operator<<=(size_t const shift)
{
	if constexpr (N == 0) {
		return *this;
	}
	if (shift == 0) {
		return *this;
	}
	if (shift < size) {
		size_t const word_shift = shift / num_bits_per_word;
		size_t const offset = shift % num_bits_per_word;
		if (offset == 0) {
			for (size_t i = num_words - 1; i >= word_shift; --i) {
				m_words[i] = m_words[i - word_shift];
			}
		} else {
			size_t const sub_offset = (num_bits_per_word - offset);
			for (size_t i = num_words - 1; i > word_shift; --i) {
				m_words[i] =
				    ((m_words[i - word_shift] << offset) |
				     (m_words[i - word_shift - 1] >> sub_offset));
			}
			m_words[word_shift] = m_words[0] << offset;
		}
		for (size_t i = 0; i < word_shift; ++i) {
			m_words[i] = static_cast<word_type>(0);
		}
		sanitize();
		return *this;
	} else {
		reset();
		return *this;
	}
}

template <size_t N, class WordType>
constexpr bitset<N, WordType> bitset<N, WordType>::operator>>(size_t const shift) const
{
	bitset ret(*this);
	return (ret >>= shift);
}

template <size_t N, class WordType>
constexpr bitset<N, WordType> bitset<N, WordType>::operator<<(size_t const shift) const
{
	bitset ret(*this);
	return (ret <<= shift);
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::shift_words_right(size_t const word_shift)
{
	if constexpr (N == 0) {
		return *this;
	}
	if (word_shift == 0) {
		return *this;
	}
	if (word_shift < num_words) {
		size_t const limit = num_words - word_shift - 1;
		for (size_t i = 0; i <= limit; ++i) {
			m_words[i] = m_words[i + word_shift];
		}
		for (size_t i = limit + 1; i < num_words; ++i) {
			m_words[i] = static_cast<word_type>(0);
		}
		return *this;
	} else {
		reset();
		return *this;
	}
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::shift_words_left(size_t const word_shift)
{
	if constexpr (N == 0) {
		return *this;
	}
	if (word_shift == 0) {
		return *this;
	}
	if (word_shift < num_words) {
		for (size_t i = num_words - 1; i >= word_shift; --i) {
			m_words[i] = m_words[i - word_shift];
		}
		for (size_t i = 0; i < word_shift; ++i) {
			m_words[i] = static_cast<word_type>(0);
		}
		sanitize();
		return *this;
	} else {
		reset();
		return *this;
	}
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::reset()
{
	for (auto& word : m_words) {
		word = static_cast<word_type>(0);
	}
	return *this;
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::reset(size_t const index)
{
	if constexpr (N > 0) {
		assert(index < size);
		size_t const word_index = index / num_bits_per_word;
		size_t const bit_index = index % num_bits_per_word;
		word_type const mask = static_cast<word_type>(1) << bit_index;

		m_words[word_index] = (m_words[word_index] & (~mask));
	}
	return *this;
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::set()
{
	for (auto& word : m_words) {
		word = ~static_cast<word_type>(0);
	}
	sanitize();
	return *this;
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::set(size_t const index, bool const value)
{
	if constexpr (N > 0) {
		assert(index < size);
		size_t const word_index = index / num_bits_per_word;
		size_t const bit_index = index % num_bits_per_word;
		word_type const mask = static_cast<word_type>(1) << bit_index;
		word_type const value_mask = static_cast<word_type>(value) << bit_index;

		m_words[word_index] = (m_words[word_index] & (~mask)) | value_mask;
	} else {
		// suppress unused but set parameter warning
		static_cast<void>(value);
	}
	return *this;
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::flip()
{
	for (auto& word : m_words) {
		word = ~word;
	}
	sanitize();
	return *this;
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>& bitset<N, WordType>::flip(size_t const index)
{
	if constexpr (N > 0) {
		assert(index < size);
		size_t const word_index = index / num_bits_per_word;
		size_t const bit_index = index % num_bits_per_word;
		word_type const mask = static_cast<word_type>(1) << bit_index;

		m_words[word_index] = (m_words[word_index] & (~mask)) | ((~m_words[word_index]) & mask);
	}
	return *this;
}

template <size_t N, class WordType>
constexpr bitset<N, WordType> bitset<N, WordType>::operator~() const
{
	bitset ret(*this);
	ret.flip();
	return ret;
}

template <size_t N, class WordType>
constexpr bool bitset<N, WordType>::test(size_t const index) const
{
	if constexpr (N > 0) {
		assert(index < size);
		size_t const word_index = index / num_bits_per_word;
		size_t const bit_index = index % num_bits_per_word;
		word_type const mask = static_cast<word_type>(1) << bit_index;

		return (m_words[word_index] & mask);
	}
	return false;
}

template <size_t N, class WordType>
constexpr bool bitset<N, WordType>::operator==(bitset const& other) const
{
	for (size_t i = 0; i < num_words; ++i) {
		if (m_words[i] != other.m_words[i]) {
			return false;
		}
	}
	return true;
}

template <size_t N, class WordType>
constexpr bool bitset<N, WordType>::operator!=(bitset const& other) const
{
	return !(*this == other);
}

template <size_t N, class WordType>
constexpr WordType bitset<N, WordType>::highest_word_bitmask() const
{
	return fill_bits<word_type>(N - ((num_words - 1) * num_bits_per_word));
}

template <size_t N, class WordType>
constexpr bool bitset<N, WordType>::all() const
{
	if constexpr (N > 0) {
		for (size_t i = 0; i < num_words - 1; ++i) {
			// double cast to avoid a sign-compare warning
			if (m_words[i] != static_cast<word_type>(~static_cast<word_type>(0))) {
				return false;
			}
		}
		return (m_words.back() == highest_word_bitmask());
	}
	return false;
}

template <size_t N, class WordType>
constexpr bool bitset<N, WordType>::none() const
{
	for (auto word : m_words) {
		if (word != static_cast<word_type>(0)) {
			return false;
		}
	}
	return true;
}

template <size_t N, class WordType>
constexpr bool bitset<N, WordType>::any() const
{
	return !none();
}

template <size_t N, class WordType>
constexpr void bitset<N, WordType>::sanitize()
{
	if constexpr (N > 0) {
		m_words.back() &= highest_word_bitmask();
	}
}

template <size_t N, class WordType>
constexpr std::array<WordType, bitset<N, WordType>::num_words> const&
bitset<N, WordType>::to_array() const
{
	return m_words;
}

template <size_t N, class WordType>
constexpr bitset<N, WordType>::
operator std::array<WordType, bitset<N, WordType>::num_words> const&() const
{
	return to_array();
}

template <size_t N, class WordType>
std::string bitset<N, WordType>::to_string() const
{
	std::ostringstream ss;
	for (size_t i = 0; i < size; ++i) {
		ss << (test(size - i - 1) ? "1" : "0");
	}
	return ss.str();
}

template <size_t N, class WordType>
bitset<N, WordType>::operator std::string() const
{
	return to_string();
}

template <size_t N, class WordType>
constexpr uintmax_t bitset<N, WordType>::to_uintmax() const
{
	return static_cast<uintmax_t>(*this);
}

template <size_t N, class WordType>
template <typename UnsignedIntegerType, typename>
constexpr bitset<N, WordType>::operator UnsignedIntegerType() const
{
	bitset<sizeof(UnsignedIntegerType) * CHAR_BIT, UnsignedIntegerType> converter(*this);
	return converter.to_array().front();
}

template <size_t N, class WordType>
constexpr unsigned long bitset<N, WordType>::to_ulong() const
{
	return static_cast<unsigned long>(*this);
}

template <size_t N, class WordType>
constexpr unsigned long long bitset<N, WordType>::to_ullong() const
{
	return static_cast<unsigned long long>(*this);
}

template <size_t N, class WordType>
std::ostream& operator<<(std::ostream& os, bitset<N, WordType> const& value)
{
	os << value.to_string();
	return os;
}

} // namespace hate
