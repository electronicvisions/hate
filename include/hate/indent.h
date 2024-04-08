#pragma once
#include "hate/visibility.h"
#include <ostream>
#include <string>

namespace hate {

struct Indentation;


/**
 * Ostream wrapper which allows additing indentation on every line-break.
 */
struct IndentingOstream
    : private std::streambuf
    , public std::ostream
{
	IndentingOstream(std::ostream& os) SYMBOL_VISIBLE;

protected:
	virtual int overflow(int c) override;
	virtual std::streamsize xsputn(const char* s, std::streamsize count) override;

private:
	friend std::ostream& operator<<(std::ostream& os, Indentation const& value) SYMBOL_VISIBLE;

	std::ostream& m_os;
	std::string m_indentation;
	bool m_insert_indentation;
};


/**
 * Add indentation to multi-line string.
 * The indentation is added after every line break.
 */
struct Indentation
{
	Indentation() = default;
	/**
	 * Construct indentation with specified value.
	 * @param value Value to add after every line break
	 */
	Indentation(std::string value) SYMBOL_VISIBLE;

	friend std::ostream& operator<<(std::ostream& os, Indentation const& value) SYMBOL_VISIBLE;

private:
	std::string m_value;
};

} // namespace hate
