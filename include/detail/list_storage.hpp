#pragma once

#include <list>
#include <optional>

#include "detail/node.hpp"

namespace efesx {
namespace queue {
namespace detail
{
    

class list_storage {
private:
    std::list<node> store;
public:
    list_storage() = default;
    list_storage(const list_storage&) = delete;
    list_storage(list_storage&&) = delete;
    list_storage& operator=(list_storage&&) = delete;
    ~list_storage() = default;

    template<typename...Args>
    void insert(Args...args){
        store.emplace_back(args...);
        store.sort();
    }

    template<typename T = bool>
    std::optional<T> extract(void* buf = nullptr){
        if(store.size() == 0) {
            return std::optional<T>{};
        }

        node& val = store.front();

        if(!val.has_value() || val.size() == 0){
            return std::optional<T>{};
        }

        if (buf != nullptr){
            val.blob_copy(buf);
            return std::optional<T>{};
        }

        try{
            return val.value_cast<T>();
        } catch (std::runtime_error& e){
            return std::optional<T>{};
        }
        
    }

    void pop(){
        store.pop_front();
    }

    template<typename T>
    std::optional<T> extract_with_type(void* buf = nullptr){
        throw std::runtime_error("Not Yet Implemented Error");
    }



};



}; // namespace detail
}; // namespace queue
}; // namespace efesxzc
