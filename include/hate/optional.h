#pragma once

#ifdef __cpp_lib_optional

#include <optional>

namespace hate {

template <typename T>
using optional = std::optional<T>;

} // namespace hate

#else

#include <experimental/optional>

namespace hate {

template <typename T>
using optional = std::experimental::optional<T>;

} // namespace hate

#endif
