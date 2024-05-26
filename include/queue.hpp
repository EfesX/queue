#pragma once

#include <list>
#include <type_traits>

#include "detail/node.hpp"


namespace efesx {

class queue {
private:
    using node = detail::node;

    std::list<node> pool;
public:
    queue() = default;
    queue(const queue&) = delete;
    queue(const queue&&) = delete;

    template<typename T>
    void enqueue(const T& element, u_int8_t priority = 0){
        pool.emplace_back(element, priority);
    }

    template<typename T>
    void dequeue(T& element){
        pool.front().restore(element);
        pool.pop_front();
    }
};


}; // namespace efesx