#pragma once

#include <string>

#include <grpcpp/grpcpp.h>

#include "proto/queue.pb.h"
#include "proto/queue.grpc.pb.h"

#include <absl/strings/str_format.h>

namespace efesx::queue::web {

using service = efesx::queue::detail::proto::QueueService;
using status  = efesx::queue::detail::proto::StatusCode;
using request = efesx::queue::detail::proto::QueueRequest;
using reply   = efesx::queue::detail::proto::QueueReply;
using node    = efesx::queue::detail::proto::QueueStorageNode;


class queue_client {
private:
    const std::string address;
    uint16_t port;

    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<service::Stub> stub;

public:
    queue_client(const std::string& _address, uint16_t _port) : address(_address), port(_port) {
        auto target_str = absl::StrFormat("%s:%d", address, port);
        channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
        stub = service::NewStub(channel);
    }
    ~queue_client() = default;

    status create_queue(const std::string& name){
        request req;
        req.set_name_queue(name);

        reply rep;
        grpc::ClientContext context;
        grpc::Status response = stub->CreateQueue(&context, req, &rep);

        if (response.ok()) {
            return rep.status();
        }

        return status::UNKNOWN_ERROR;     
    }

    status delete_queue(const std::string& name){
        request req;
        req.set_name_queue(name);

        reply rep;
        grpc::ClientContext context;

        grpc::Status response = stub->DeleteQueue(&context, req, &rep);

        if (response.ok()) {
            return rep.status();
        }

        return status::UNKNOWN_ERROR;
    }

    status push_data(const std::string& name, node* _node){
        request req;
        req.set_name_queue(name);
        req.set_allocated_node(_node);

        reply rep;
        grpc::ClientContext context;
        grpc::Status response = stub->PushData(&context, req, &rep);

        if (response.ok()) {
            return rep.status();
        }

        return status::UNKNOWN_ERROR;     
    }

    status pop_data(const std::string& name, std::shared_ptr<node>& _node){
        request req;
        req.set_name_queue(name);

        reply rep;
        grpc::ClientContext context;
        grpc::Status response = stub->PopData(&context, req, &rep);

        if (response.ok() && rep.has_node()) {
            _node = std::make_shared<node>(rep.node());
            return rep.status();
        }

        return status::UNKNOWN_ERROR;     
    }

/*
    rpc CreateQueue (QueueRequest) returns (QueueReply) {}
    rpc DeleteQueue (QueueRequest) returns (QueueReply) {}
    rpc PushData    (QueueRequest) returns (QueueReply) {}
    rpc PopData     (QueueRequest) returns (QueueReply) {}
*/    

};


}; // namespace efesx::queue::web
