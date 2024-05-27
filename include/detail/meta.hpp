#pragma once

#include <type_traits>
#include <concepts>
#include <string>

#include "types.hpp"

namespace efesx {
namespace queue {
namespace detail {

//@brief Structure for convertig T to value_t
template<typename T>
struct detector {
    inline static constexpr value_t type = value_t::null_t;
};
template<>
struct detector<int>{
    inline static constexpr value_t type = value_t::int_t;
};
template<>
struct detector<bool>{
    inline static constexpr value_t type = value_t::bool_t;
};
template<>
struct detector<std::string>{
    inline static constexpr value_t type = value_t::string_t;
};
template<>
struct detector<char>{
    inline static constexpr value_t type = value_t::char_t;
};

template<typename T>
concept Container = 
    requires { typename T::iterator::iterator_category::forward_iterator_tag; } && 
    !std::is_same_v<T, std::string> && 
    requires (T a) {
        { a.begin()++ };
        { a.end() };
        { a.size() } -> std::convertible_to<std::size_t>;
    };

}; // namespace detail
}; // namespace queue
}; // namespace efesx
