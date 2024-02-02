#pragma once
#include <boost/type_index.hpp>

namespace hate {

/**
 * Get fully qualified name of a type including namespaces.
 * The format follows "name::space::Type::SubType<TParams...>".
 * @tparam T Type to get name for
 */
template <typename T>
std::string full_name()
{
	return boost::typeindex::type_id<T>().pretty_name();
}

/**
 * Get name of a type stripped of namespaces and surrounding classes.
 * The format follows "SubType<TParams...>" for name::space::Type::SubType<Tparams...>.
 * @tparam T Type to get name for
 */
template <typename T>
std::string name()
{
	auto const full = full_name<T>();
	return full.substr(full.rfind(':', full.find('<')) + 1);
}

} // namespace hate
