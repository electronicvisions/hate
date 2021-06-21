#pragma once
#include <algorithm>
#include <string>

namespace hate {

/**
 * Add indentation to multi-line string.
 * The indentation is added after every line break.
 * @param value String value to indent
 * @param indentation Indentation to use
 */
inline std::string indent(std::string value, std::string const& indentation)
{
	size_t start_pos = 0;
	std::string const from{"\n"};
	std::string const to{"\n" + indentation};
	while ((start_pos = value.find(from, start_pos)) != std::string::npos) {
		value.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return indentation + value;
}

} // namespace hate
