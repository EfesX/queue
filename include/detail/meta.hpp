#pragma meta

#include <type_traits>
#include <concepts>
#include <string>

namespace detail {
    template<typename T>
    concept Trivial = std::is_trivial_v<std::remove_cv_t<T>> && !std::is_bounded_array_v<T>;

    template<typename T>
    concept String = std::convertible_to<T, std::string>;

    template<typename T>
    concept TrivialOrString = Trivial<T> || String<T>;

    template<typename T>
    concept Array = requires(T* arr) {
        arr[0];
    };
};
