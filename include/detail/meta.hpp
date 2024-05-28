#pragma once

#include <type_traits>
#include <concepts>
#include <string>

#include "types.hpp"

namespace efesx {
namespace queue {
namespace detail {

template<typename T>
concept IsIntegral = std::is_integral_v<T>;

template<typename T>
concept IsFloating = std::is_floating_point_v<std::remove_cv_t<T>>;

template<typename T>
concept IsString = std::is_convertible_v<T, std::string>;

}; // namespace detail
}; // namespace queue
}; // namespace efesx
