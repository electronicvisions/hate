#pragma once
#include <array>
#include <memory>
#include <stdexcept>

namespace hate {

/**
 * Wrapper for storage, access and alteration-tracking to an object with explicitly saved history.
 * Aside the current object, (configurable many) snapshot instances are saved and can be accessed.
 * Read-only access to the current instance adheres to a pointer interface, write-access is explicit
 * in order to track occurrence of write accesses.
 */
template <typename T, size_t Depth = 1>
class HistoryWrapper
{
public:
	typedef T value_type;
	constexpr static size_t depth = Depth;

	/**
	 * Default construct HistoryWrapper and default-construct current value.
	 * History storage is allocated on demand.
	 */
	HistoryWrapper() : m_value(std::make_unique<value_type>()), m_value_history() {}

	/**
	 * Pointer access to current instance.
	 * return Pointer to current value
	 */
	value_type* operator->() const { return m_value.operator->(); }

	/**
	 * operator* to reference of current value.
	 * @return Reference to current value
	 */
	value_type& operator*() const { return *m_value; }

	/**
	 * Save current value to history.
	 * If the specified history depth is reached, the oldest value is discarded and all other moved
	 * one position.
	 */
	void save()
	{
		for (size_t i = depth - 1; i > 0; --i) {
			m_value_history[i].swap(m_value_history[i - 1]);
		}
		if (!m_value_history.front()) {
			m_value_history.front() = std::make_unique<value_type>(*m_value);
		} else {
			*m_value_history.front() = *m_value;
		}
	}

	/**
	 * Check whether history entry at specified position exists.
	 * A larger index corresponds to older value.
	 * @param index Index of history to query existence for
	 */
	bool has_history(size_t const index = 0) const
	{
		return static_cast<bool>(m_value_history.at(index));
	}

	/**
	 * Get constant reference to history entry at specified position.
	 * @param index Index of history to query
	 * @throws std::runtime_error On history entry not (yet) filled
	 */
	value_type const& get_history(size_t const index = 0) const
	{
		if (!m_value_history.at(index)) {
			throw std::runtime_error("Trying to access non-existent history.");
		}
		return *(m_value_history.at(index));
	}

private:
	std::unique_ptr<T> m_value;
	std::array<std::unique_ptr<T>, Depth> m_value_history;

	static_assert(Depth > 0, "History depth can't be zero.");
};

} // namespace hate
