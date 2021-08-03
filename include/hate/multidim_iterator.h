#pragma once
#include <cstdint>
#include <vector>

namespace hate {

struct MultidimIterator
{
	MultidimIterator(std::vector<int64_t> const& range) :
	    m_range(range), m_state(m_range.size(), static_cast<int64_t>(0))
	{}

	inline MultidimIterator& operator++()
	{
		if (m_state.empty()) {
			return *this;
		}
		auto const stop_state = *end();
		m_state.back()++;
		for (size_t i = 0; i < m_state.size(); ++i) {
			size_t ri = m_state.size() - i - 1;
			if (((ri == m_state.size() - 1) &&
			     !std::equal(m_state.begin(), m_state.end() - 1, stop_state.begin())) ||
			    (i != 0 && ri > 0)) {
				if (m_state.at(ri) == m_range.at(ri)) {
					m_state.at(ri) = 0;
					m_state.at(ri - 1)++;
				}
			}
		}
		return *this;
	}

	inline MultidimIterator end() const
	{
		MultidimIterator ret(m_range);
		ret.m_state = m_range;
		if (!m_range.empty()) {
			for (size_t i = 0; i < m_range.size() - 1; ++i) {
				ret.m_state.at(i)--;
			}
		}
		return ret;
	}

	inline std::vector<int64_t> const& operator*() const
	{
		return m_state;
	}

	inline bool operator==(MultidimIterator const& other) const
	{
		return (m_range == other.m_range) && (m_state == other.m_state);
	}

	inline bool operator!=(MultidimIterator const& other) const
	{
		return !(*this == other);
	}

private:
	std::vector<int64_t> m_range;
	std::vector<int64_t> m_state;
};

} // namespace hate
