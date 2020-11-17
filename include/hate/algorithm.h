#pragma once
#include <set>
#include <vector>

namespace hate {

template <typename T>
bool has_intersection(std::set<T> const& a, std::set<T> const& b)
{
	std::set<T> all(a);
	all.insert(b.begin(), b.end());
	return all.size() != (a.size() + b.size());
}

template <typename T>
bool has_intersection(std::vector<T> const& a, std::vector<T> const& b)
{
	std::set<T> aa(a.begin(), a.end());
	std::set<T> bb(b.begin(), b.end());
	return has_intersection(aa, bb);
}

} // namespace hate
