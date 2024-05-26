#pragma once

#include <list>
#include <type_traits>

#include "detail/node.hpp"


namespace efesx {
    //using namespace detail;

class queue {
private:
    using node_t = detail::node;

    std::list<node_t> pool;
public:
    queue() = default;
    queue(const queue&) = delete;
    queue(const queue&&) = delete;

    template<detail::TrivialOrString T>
    void enqueue(const T& element, u_int8_t priority = 0){
        pool.emplace_back(element, priority);
        if(priority > 0){
            pool.sort();
        }
    }

    template<detail::Array T>
    void enqueue(T* element, std::size_t size, u_int8_t priority = 0){
        pool.emplace_back(element, size, priority);
        if(priority > 0){
            pool.sort();
        }
    }

    template<detail::TrivialOrString T>
    void dequeue(T& element){
        pool.front().restore(element);
        pool.pop_front();
    }

    template<detail::Array T>
    void dequeue(T* element){
        pool.front().restore(element);
        pool.pop_front();
    }
};


}; // namespace efesx