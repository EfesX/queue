#pragma once

#include <type_traits>
#include <concepts>
#include <string>

namespace efesx {
namespace queue {
namespace detail {

template<typename T>
concept IsBoolean = std::is_same_v<T, bool>;

template<typename T>
concept IsIntegral = std::is_integral_v<T>;

template<typename T>
concept IsSigned = std::is_integral_v<T> && std::is_signed_v<T>;

template<typename T>
concept IsUnsigned = std::is_integral_v<T> && std::is_unsigned_v<T>;

template<typename T>
concept IsFloating = std::is_floating_point_v<std::remove_cv_t<T>>;

template<typename T>
concept IsString = std::is_convertible_v<T, std::string>;

}; // namespace detail
}; // namespace queue
}; // namespace efesx
