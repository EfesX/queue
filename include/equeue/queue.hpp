#pragma once

#include <list>
#include <type_traits>

#include "equeue/detail/proto_storage.hpp"


namespace efesx {
namespace queue {

using storage_t    = detail::proto_storage;

using node_t       = storage_t::node;
using node_p_t     = storage_t::node_ptr;
using node_value_t = node_t::DataCase;

template<typename Storage>
class basic_queue {
private:
    Storage storage;
public:
    basic_queue() = default;
    ~basic_queue() = default;

    basic_queue(const basic_queue&)  = delete;
    basic_queue(basic_queue&&)  = delete;
    basic_queue& operator=(basic_queue&&) = delete;
    basic_queue operator=(const basic_queue&) = delete;

    /**
     * @brief Put any (any standard type in C++, except classes, structs and enums) data to queue.
    */
    template<typename...Args>
    void enqueue(Args...args){
        storage.insert(args...);
    }

    /**
     * @brief Get top data node from queue
     * @return node_p_t& - the reference to shared pointer of data node in queue
    */
    node_p_t& dequeue(){
        return storage.extract();
    }

    /**
     * @brief Delete delete top data node from queue
    */
    void pop(){
        storage.pop();
    }

    /**
     * @brief Returns amount of data nodes in queue
    */
    std::size_t amount(){
        return storage.amount();
    }

    /**
     * @brief Is queue empty?
    */
    bool empty(){
        return storage.empty();
    }

    /**
     * @brief Serialize queue to file
    */
    bool save_to_disk(const std::string& file){
        return storage.save(file);
    }

    /**
     * @brief Deserialize queue to file
    */
    bool load_from_disk(const std::string& file){
        return storage.load(file);
    }

    bool operator==(const basic_queue<Storage>& oth) const {
        return (storage == oth.storage);
    }

};

using queue = basic_queue<storage_t>;


}; // namespace queue
}; // namespace efesx
