#pragma once

#include <list>
#include <type_traits>

#include "detail/node.hpp"
#include "detail/list_storage.hpp"


namespace efesx {
namespace queue {

template<typename Storage>
class basic_queue {
private:
    Storage storage;
public:
};


using queue = basic_queue<detail::list_storage>;


}; // namespace queue
}; // namespace efesx