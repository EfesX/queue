#pragma once

#include <list>
#include <optional>
#include <memory>
#include <functional>

#include "detail/node.hpp"
#include "detail/meta.hpp"

namespace efesx {
namespace queue {
namespace detail
{
    

class list_storage {
private:
    using store_t = std::list<node>;

    store_t store;

    struct accesser {
        store_t& m_store;
        store_t::iterator& m_it;

        accesser(store_t& _store, store_t::iterator& _it) : 
            m_store(_store), 
            m_it(_it)
        {}
        ~accesser(){
            m_store.erase(m_it);
        }

        template<typename T>
        std::pair<T, bool> access(void* buf = nullptr){
            if(buf != nullptr){
                m_it->blob_copy(buf);
                return std::make_pair(T{}, true);
            }
            return std::make_pair(m_it->value_cast<T>(), true);
        }
    };

    template<typename T>
    std::pair<T, bool> _get_value(value_t t, void* buf = nullptr){
        for (auto it = store.begin(); it != store.end(); it++){
            if (it->value_type() == t) {
                if (it->has_value() && (it->size() != 0)){
                    // to automaticaly erase node from store (in destructor of accesser)
                    std::unique_ptr<accesser> obj = std::make_unique<accesser>(store, it);
                    return obj->access<T>(buf);
                }
            }
        }
        return std::make_pair(T{}, false);
    }

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

    bool empty(){
        return (store.size() == 0);
    }

    template<typename T = bool>
    std::optional<T> extract(void* buf = nullptr){
        if(empty()) return std::optional<T>{};

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
    std::pair<T, bool> extract_with_type(){
        if(empty()) return std::make_pair(T{}, false);

        if constexpr (IsIntegral<T>)
        {
            return _get_value<T>(value_t::integer);    
        } 
        else if constexpr (IsFloating<T>)
        {
            return _get_value<T>(value_t::floating); 
        }
        else if constexpr (IsString<T>)
        {
            return _get_value<T>(value_t::text);    
        }
        else
        {
            throw std::runtime_error("Not Implemented Error");
        }
        return std::make_pair(T{}, false);
    }

    std::pair<void*, bool> extract_with_type(void* buf = nullptr){
        if (buf == nullptr)
        {
            return std::make_pair(nullptr, false);
        }
        return _get_value<void*>(value_t::blob, buf);
    }
};



}; // namespace detail
}; // namespace queue
}; // namespace efesxzc
