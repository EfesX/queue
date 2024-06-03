#pragma once

#include <shared_mutex>
#include <any>

#include "detail/proto_storage.hpp"
#include "queue.hpp"



using namespace std::chrono_literals;

namespace efesx::queue_async {

using storage_t    = queue::detail::proto_storage;

using node_t       = storage_t::node_t;
using node_p_t     = storage_t::node_p_t;
using node_value_t = node_t::DataCase;

template<typename BasicQueue>
class basic_queue_async {
private:
    BasicQueue queue;
    std::shared_mutex mtx;

public:
    basic_queue_async() = default;
    ~basic_queue_async() = default;

    basic_queue_async(const basic_queue_async&)  = delete;
    basic_queue_async(basic_queue_async&&)  = delete;
    basic_queue_async& operator=(basic_queue_async&&) = delete;
    basic_queue_async operator=(const basic_queue_async&) = delete;

    /**
     * @brief Put any (any standard type in C++, except classes, structs and enums) data to queue.
    */
    template<typename...Args>
    void enqueue(Args...args){
        std::unique_lock lck(mtx);
        queue.enqueue(args...);
    }

    /**
     * @brief Get top data node from queue
     * @return node_p_t& - the reference to shared pointer of data node in queue
    */
    std::any dequeue(bool _pop = false){
        std::unique_lock lock(mtx);

        if (queue.empty()) return std::any();
        
        node_p_t res = queue.dequeue();

        if(_pop) queue.pop();

        return res;
    }

    /**
     * @brief Delete delete top data node from queue
    */
    void pop(){
        std::unique_lock lck(mtx);
        if (queue.empty()) return;
        queue.pop();
    }

    /**
     * @brief Returns amount of data nodes in queue
    */
    std::size_t amount(){
        std::shared_lock lck(mtx);
        return queue.amount();
    }

    /**
     * @brief Is queue empty?
    */
    bool empty(){
        std::shared_lock lck(mtx);
        return queue.empty();
    }

    /**
     * @brief Serialize queue to file
    */
    bool save_to_disk(const std::string& file){
        std::shared_lock lck(mtx);
        return queue.save_to_disk(file);
    }

    /**
     * @brief Deserialize queue to file
    */
    bool load_from_disk(const std::string& file){
        std::unique_lock lck(mtx);
        return queue.load_from_disk(file);
    }

    bool operator==(const basic_queue_async<BasicQueue>& oth) const {
        return (queue == oth.queue);
    }
};

using queue_async = basic_queue_async<efesx::queue::basic_queue<storage_t>>;

}; // namespace efesx::queue_async
