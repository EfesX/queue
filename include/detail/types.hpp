#pragma once

#include <stdint.h>
#include <iostream>

namespace efesx {
namespace queue {
namespace detail {

enum value_t {
    null = 0,
    integer,
    floating,
    text,
    blob
};


}; // namespace detail
}; // namespace queue
}; // namespace detail
