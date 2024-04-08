#include "hate/indent.h"

#include <ostream>
#include <stdexcept>
#include <string>

namespace hate {

IndentingOstream::IndentingOstream(std::ostream& os) :
    std::ostream(this), m_os(os), m_indentation(), m_insert_indentation(true)
{}

int IndentingOstream::overflow(int c)
{
	if (m_insert_indentation) {
		m_os << m_indentation;
		m_insert_indentation = false;
	}
	if (static_cast<char>(c) == '\n') {
		m_insert_indentation = true;
	}
	m_os.put(c);
	return 0;
}

std::streamsize IndentingOstream::xsputn(const char* s, std::streamsize count)
{
	size_t begin = 0;
	size_t end = 0;
	for (std::streamsize i = 0; i < count; ++i) {
		if (m_insert_indentation) {
			m_os.write(s + begin, end - begin);
			m_os << m_indentation;
			m_insert_indentation = false;
			begin = i;
		}
		m_insert_indentation = (s[i] == '\n');
		end = i + 1;
	}
	m_os.write(s + begin, end - begin);
	return count;
}


Indentation::Indentation(std::string value) : m_value(std::move(value)) {}

std::ostream& operator<<(std::ostream& os, Indentation const& value)
{
	if (auto const ptr = dynamic_cast<IndentingOstream*>(&os); ptr) {
		ptr->m_indentation = value.m_value;
	} else {
		throw std::logic_error("Indentation requires IndentingOstream.");
	}
	return os;
}

} // namespace hate
