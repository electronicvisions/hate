#pragma once

#include <atomic>
#include <chrono>
#include <type_traits>
#include <utility>

namespace hate {

/**
 * Simple wrapper that tracks whenever the wrapped object gets modified.
 *
 * Because on a non-const object, the compiler will always choose the non-const
 * version of a method, we cannot overload `operator*()`.
 *
 * Hence we use `operator*()` for read-only modified and request modification via `get()`.
 */
template <typename T>
class TrackModifications
{
public:
	using value_type = T;

	template <typename... Args>
	explicit TrackModifications(Args&&... args) : m_wrapped(std::forward<Args>(args)...)
	{
		modified();
	}

	TrackModifications(TrackModifications&& other) : m_wrapped(std::move(other.m_wrapped))
	{
		modified();
	}

	TrackModifications(TrackModifications const& other) : m_wrapped(other.m_wrapped)
	{
		modified();
	}

	TrackModifications& operator=(TrackModifications const& other)
	{
		modified();
		m_wrapped = other.m_wrapped;
		return *this;
	}

	TrackModifications& operator=(TrackModifications&& other)
	{
		modified();
		m_wrapped = std::move(other.m_wrapped);
		return *this;
	}

	/**
	 * Request to modify wrapped value.
	 */
	std::add_lvalue_reference_t<value_type> get()
	{
		modified();
		return m_wrapped;
	}

	/**
	 * Get read-only modified to wrapped value. modified is not tracked.
	 */
	std::add_lvalue_reference_t<value_type const> operator*() const
	{
		return m_wrapped;
	}

	/**
	 * Get elapsed time since last modification.
	 */
	auto elapsed() const
	{
		return std::chrono::system_clock::now() - m_last_access.load(std::memory_order_acquire);
	}

	/**
	 * Check if the given duration has elapsed
	 */
	template <typename Duration>
	bool is_elapsed(Duration const& duration) const
	{
		return elapsed() > duration;
	}

private:
	void modified()
	{
		m_last_access.store(std::chrono::system_clock::now(), std::memory_order_release);
	}

	std::atomic<std::chrono::system_clock::time_point> m_last_access;
	value_type m_wrapped;
};

} // namespace hate
