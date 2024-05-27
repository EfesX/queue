#pragma once

#include "types.hpp"
#include "meta.hpp"

namespace efesx {
namespace queue {
namespace detail {

class node {
private:
union node_value
{
    int int_value;
    bool bool_value;
    char char_value;
    std::string* string_value;
    std::vector<node>* array_value;

    std::size_t v_size;

    node_value() = default;
    node_value(const node_value&) = default;
    node_value(node_value&& oth) = default;

    node_value(int v) : int_value(v) {}
    node_value(bool v) : bool_value(v) {}
    node_value(char v) : char_value(v) {}

    node_value(value_t t, std::size_t size) {
        switch (t)
        {
        case value_t::container_t: {
            array_value = create<std::vector<node>>();
            array_value->reserve(size);
            break;
        }
        
        default: break;
        }
    }

    node_value(const std::string& v) : string_value(create<std::string>(v)) {}
    node_value(const std::string&& v) : string_value(create<std::string>(std::move(v))) {}

    template<typename T, std::enable_if_t<std::is_pointer_v<T>, int> = 0>
    node_value(T arr, std::size_t size) {
        array_value = create<std::vector<node>>();
        array_value->reserve(size);

        for (auto i = 0; i < size; i++){
            array_value->emplace_back(arr[i]);
        }
    }

    template<typename T, typename... Args>
    static T* create(Args&& ... args){
        using allocator_traits = std::allocator_traits<std::allocator<T>>;

        std::allocator<T> alloc;
        auto deleter = [&](T* obj){
            allocator_traits::deallocate(alloc, obj, 1);
        };
        std::unique_ptr<T, decltype(deleter)> obj(allocator_traits::allocate(alloc, 1), deleter);
        allocator_traits::construct(alloc, obj.get(), std::forward<Args>(args)...);
        return obj.release();
    }

    void destroy(value_t t) {
        switch (t)
        {
        case value_t::string_t: {
            std::allocator<std::string> alloc;
            std::allocator_traits<decltype(alloc)>::destroy(alloc, string_value);
            std::allocator_traits<decltype(alloc)>::deallocate(alloc, string_value, 1);
            break;
        }
        case value_t::array_t: {
            std::allocator<std::vector<node>> alloc;
            std::allocator_traits<decltype(alloc)>::destroy(alloc, array_value);
            std::allocator_traits<decltype(alloc)>::deallocate(alloc, array_value, 1);            
            break;
        }
            
        default: break;
        }
    }
};
public:
    value_t m_type;
    value_t m_subtype;

    node_value m_data;

    // ARRAY
    template<typename T, std::enable_if_t<std::is_pointer_v<T>, int> = 0> requires (!Container<T>)
    node(const T val, std::size_t size, std::size_t priority = 0) : 
        m_data(val, size), 
        m_type(value_t::array_t),
        m_subtype(detector<std::remove_const_t<std::remove_pointer_t<T>>>::type) 
    {}

    // ANOTHER ALL
    template<typename T, std::enable_if_t<std::is_fundamental_v<T> && !std::is_pointer_v<T>, int> = 0>
    node(T val, std::size_t priority = 0) : 
        m_data(val), 
        m_type(detector<T>::type) 
    {}

    // STRING
    template<typename T, std::enable_if_t<std::is_same_v<std::string, std::remove_cv_t<T>>, int> = 0>
    node(T val, std::size_t priority = 0) : 
        m_data(val), 
        m_type(detector<T>::type)
    {}

    // CONTAINERS
    template<Container T>
    node(const T& container, std::size_t priority = 0) : 
        m_data(value_t::container_t, container.size()),
        m_type(value_t::container_t)
    {
        for (auto it = container.begin(); it != container.end(); it++){
            m_data.array_value->emplace_back(*it, priority);
        }
    }

    node(const node&) = default;
    node(node&& oth) = default;

    ~node() { 
        m_data.destroy(m_type); 
    }

    // ANOTHER ALL
    template<typename T, std::enable_if_t<std::is_fundamental_v<T> && !std::is_pointer_v<T>, int> = 0>
    void restore(T& data){
        data = m_data.int_value;
    }

    // STRING
    template<typename T, std::enable_if_t<std::is_same_v<std::string, std::remove_cv_t<T>>, int> = 0>
    void restore(T& data){
        data = *m_data.string_value;
    }

    // ARRAY
    template<typename T, std::enable_if_t<std::is_pointer_v<T>, int> = 0> requires (!Container<T>)
    void restore(T val) {
        std::size_t idx = 0;
        for (auto it = m_data.array_value->begin(); it != m_data.array_value->end(); it++){
            it->restore(val[idx]);
            idx++;
        }
    }
};

}; // namespace detail
}; // namespace queue
}; // namespace efesx
