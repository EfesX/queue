#pragma once

#include "meta.hpp"

namespace detail {

enum class value_t : u_int8_t
{
    null,  
    array, 
    string,
    trivial,
};

class node {
private:
    using value_t = detail::value_t;

    value_t m_type;
    void* m_data;
    std::size_t m_size;
    node* next = nullptr;

public:
    template<Trivial T>
    node(const T& value, std::size_t size = sizeof(T)){
        m_data = new T{value};
        m_type = value_t::trivial;
        m_size = size;
    }

    template<String T>
    node(const T& value){
        m_data = new T{value};
        m_type = value_t::string;
        m_size = value.size();
    }

    template<typename T>
    node(const T* array, std::size_t size){
        m_type = value_t::array;
        m_size = size;

        node* _next = this;

        for (auto i = 0; i < m_size; i++){
            _next->next = new node(array[i]);
            _next = _next->next;
        }
    }


    template<Trivial T>
    void restore(T& value){
        value = *static_cast<T*>(m_data);
        delete static_cast<T*>(m_data);
    }

    template<String T>
    void restore(T& value){
        value = *static_cast<T*>(m_data);
        delete static_cast<T*>(m_data);
    }

    template<typename T>
    void restore(T* value){
        auto _next = this->next;

        for (auto i = 0; i < m_size; i++){
            if (_next != nullptr){
                value[i] = *static_cast<T*>(_next->m_data);
                _next = _next->next;
            } else {
                break;
            }
        }
    }
};


}; // namespace detail

