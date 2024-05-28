#pragma once

#include <cstring>

#include "types.hpp"
#include "meta.hpp"

namespace efesx {
namespace queue {
namespace detail {

struct node {
private:
    const uint8_t m_priority = 0;
    const value_t m_type;
    const std::size_t m_size;
    void* m_data;

    template<typename T, typename... Args>
    T* create(Args&& ... args){
        using allocator_traits = std::allocator_traits<std::allocator<T>>;
        std::allocator<T> alloc;


        auto deleter = [&](T* obj){
            allocator_traits::deallocate(alloc, obj, 1);
        };

        std::unique_ptr<T, decltype(deleter)> obj(allocator_traits::allocate(alloc, 1), deleter);
        allocator_traits::construct(alloc, obj.get(), std::forward<Args>(args)...);
        return obj.release();
    }

    template<typename T>
    void destroy(T* mem, std::size_t size){
        std::allocator<T> alloc;
        std::allocator_traits<decltype(alloc)>::destroy(alloc, mem);
        std::allocator_traits<decltype(alloc)>::deallocate(alloc, mem, size);
        m_data = nullptr;
    }
public:
    node() : m_type(value_t::null), m_size(0), m_data(nullptr) {}

    node(const node&) = delete;
    node(node&&) = delete;
    node& operator=(node&&) = delete;
    ~node(){
        if (m_type == value_t::blob) {
            std::free(m_data);
            return;
        }
        destroy<uint8_t>(static_cast<uint8_t*>(m_data), m_size);
    }

    template<IsIntegral T, uint8_t Size = sizeof(T)>
    node(const T& val) : m_size(Size), m_type(value_t::integer), m_data(create<T>(val)) 
    {}

    template<IsFloating T, uint8_t Size = sizeof(T)>
    node(const T& val) : m_size(Size), m_type(value_t::floating), m_data(create<T>(val)) 
    {}

    template<IsString T>
    node(const T& val) : m_size(val.size()), m_type(value_t::text), m_data(create<T>(val)) 
    {}

    node(const char* val, std::size_t _size) : m_size(_size), m_type(value_t::text), m_data(create<std::string>(std::string(val, _size))) 
    {}

    node(const void* val, std::size_t _size) : m_size(_size), m_type(value_t::blob), m_data(std::malloc(_size)) {
        std::memcpy(m_data, val, _size);
    }

    template<typename T>
    T&& value_cast(){
        if (m_data == nullptr){
            throw std::runtime_error("Node Value Is Empty");
        }

        if ((IsIntegral<T> && (m_type != value_t::integer)) ||
           (IsFloating<T> && (m_type != value_t::floating))||
           (IsString<T> && (m_type != value_t::text))) 
        {
            throw std::runtime_error("Bad Node Value Cast");
        }

        return std::forward<T&&>(*static_cast<T*>(m_data));
    }
   

    void blob_copy(void *dst){
        std::memcpy(dst, m_data, m_size);
    }

    std::size_t size(){
        return m_size;
    }

    bool has_value(){
        return (m_data != nullptr);
    }

    value_t value_type(){
        return m_type;
    }

    uint8_t priority(){
        return m_priority;
    }

    bool operator<(node& rhs){
        return (m_priority > rhs.m_priority);
    }
};







}; // namespace detail
}; // namespace queue
}; // namespace efesx
