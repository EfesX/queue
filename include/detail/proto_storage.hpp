#pragma once

#include "proto/queue.pb.h"
#include <google/protobuf/util/time_util.h>

#include <list>
#include <memory>
#include <fstream>

#include "meta.hpp"

namespace efesx::queue::detail {

using namespace efesx::queue::detail::proto;

using node_t   = QueueStorageNode;
using node_p_t = std::shared_ptr<QueueStorageNode>;

bool operator<(const node_p_t lhs, const node_p_t rhs){
    return (lhs->priority() < rhs->priority());
}

bool operator==(const node_p_t lhs, const node_p_t rhs){
    if (lhs->priority() != rhs->priority()) return false;
    if (lhs->created_at() != rhs->created_at()) return false;
    
    if (lhs->has_double_value()){
        if(!rhs->has_double_value()) return false;
        if(lhs->double_value() != rhs->double_value()) return false;
    } 
    else if (lhs->has_float_value()){
        if(!rhs->has_float_value()) return false;
        if(lhs->float_value() != rhs->float_value()) return false;
    } else if (lhs->has_sint32_value()){
        if(!rhs->has_sint32_value()) return false;
        if(lhs->sint32_value() != rhs->sint32_value()) return false;
    } else if (lhs->has_sint64_value()){
        if(!rhs->has_sint64_value()) return false;
        if(lhs->sint64_value() != rhs->sint64_value()) return false;
    } else if (lhs->has_uint32_value()){
        if(!rhs->has_uint32_value()) return false;
        if(lhs->uint32_value() != rhs->uint32_value()) return false;
    } else if (lhs->has_uint64_value()){
        if(!rhs->has_uint64_value()) return false;
        if(lhs->uint64_value() != rhs->uint64_value()) return false;
    } else if (lhs->has_bool_value()){
        if(!rhs->has_bool_value()) return false;
        if(lhs->bool_value() != rhs->bool_value()) return false;
    } else if (lhs->has_string_value()){
        if(!rhs->has_string_value()) return false;
        if(lhs->string_value() != rhs->string_value()) return false;
    } else if (lhs->has_raw_data()){
        if(!rhs->has_raw_data()) return false;
        if(lhs->raw_data() != rhs->raw_data()) return false;
    }

    return true;
}

class proto_storage
{
private:
    std::list<node_p_t> store;

public:
    proto_storage() = default;
    ~proto_storage() = default;

    void insert(u_int32_t priority, const void* raw, std::size_t size){
        using google::protobuf::util::TimeUtil;

        store.emplace_front(std::make_shared<node_t>());
        auto _node = store.front();

        _node->set_raw_data(raw, size);

        *_node->mutable_created_at() = TimeUtil::SecondsToTimestamp(time(NULL));

        if(priority != 0) store.sort();
    }

    template<typename T, std::size_t Size = sizeof(T)>
    void insert(u_int32_t priority, const T& value){
        using google::protobuf::util::TimeUtil;

        store.emplace_front(std::make_shared<node_t>());
        auto _node = store.front();

        _node->set_priority(priority);

        if constexpr (IsString<T>)
        {
            _node->set_string_value(value);
        }
        else if constexpr (IsBoolean<T>)
        {
            _node->set_bool_value(value);
        }
        else if constexpr (IsSigned<T>)
        {
            if constexpr (Size == 8) _node->set_sint64_value(value);
            else _node->set_sint64_value(value);
        } 
        else if constexpr (IsUnsigned<T>)
        {
            if constexpr (Size == 8) _node->set_uint64_value(value);
            else _node->set_uint32_value(value);
        } 
        else 
        {
            static_assert(false, "Not Implemented Error");
        }

        *_node->mutable_created_at() = TimeUtil::SecondsToTimestamp(time(NULL));

        if(priority != 0) store.sort();
    }

    bool save(const std::string& file){
        GOOGLE_PROTOBUF_VERIFY_VERSION;

        QueueStorage pb_store;

        for (auto it = store.rbegin(); it != store.rend(); it++){
            node_t* _node = pb_store.add_node();
            // is it really nead to copy? may be to swap and to erase?
            _node->CopyFrom(*it->get());
        }

        std::fstream out(file, std::ios::out | std::ios::trunc | std::ios::binary);
        if (!pb_store.SerializeToOstream(&out)) {
            out.close();
            return false;
        }
        out.close();
        
        google::protobuf::ShutdownProtobufLibrary();
        return true;
    }

    bool load(const std::string& file){
        GOOGLE_PROTOBUF_VERIFY_VERSION;

        std::fstream in(file, std::ios::in | std::ios::binary);
        if (!in) return false;

        QueueStorage pb_store;

        if (!pb_store.ParseFromIstream(&in)){
            in.close();
            return false;
        } 
        in.close();

        for (int i = 0; i < pb_store.node_size(); i++) {
            store.emplace_front(std::make_shared<node_t>(pb_store.node(i)));
        }

        google::protobuf::ShutdownProtobufLibrary();
        return true;
    }

    bool operator==(const proto_storage& oth) const {
        if (store.size() != oth.store.size()) return false;

        auto it1 = oth.store.begin();
        for (auto it2 = store.begin(); it2 != store.end(); ++it2){
            if (*it2 != *it1) return false;
            ++it1;
        }

        return true;
    }
};

}; // namespace efesx::queue::detail
        