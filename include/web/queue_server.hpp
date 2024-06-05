#pragma once

#include <map>
#include <memory>
#include <thread>
#include <functional>

#include <grpcpp/grpcpp.h>
//#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>

#include <absl/strings/str_format.h>

#include "ts_queue.hpp"
#include "proto/queue.pb.h"
#include "proto/queue.grpc.pb.h"


namespace efesx::queue::web {

using ts_queue     = efesx::ts_queue::ts_queue;
using queue_p_node = efesx::ts_queue::node_p_t;


using service = efesx::queue::detail::proto::QueueService;
using status  = efesx::queue::detail::proto::StatusCode;
using request = efesx::queue::detail::proto::QueueRequest;
using reply   = efesx::queue::detail::proto::QueueReply;

using efesx::queue::detail::proto::QueueStorageNode;

class QueueServiceImpl final : public service::Service {
private:
    std::map<std::string, std::unique_ptr<ts_queue>>& pool;
    QueueStorageNode _node;
public:
    QueueServiceImpl(std::map<std::string, std::unique_ptr<ts_queue>>& _pool) : pool(_pool){}

    grpc::Status CreateQueue(
        grpc::ServerContext* _context, 
        const request* _request,
        reply* _reply
    ) override {
        bool found = (pool.find(_request->name_queue()) != pool.end());

        if(found == false){
            pool[_request->name_queue()] = std::make_unique<ts_queue>();
        }
            
        auto statuscode = found ? status::UNKNOWN_ERROR : status::SUCCESS;

        _reply->set_status(statuscode);
        return grpc::Status::OK;
    }

    grpc::Status DeleteQueue(
        grpc::ServerContext* _context, 
        const request* _request,
        reply* _reply
    ) override {
        auto it = pool.find(_request->name_queue());
        if (it != pool.end()){
            pool.erase(it);
            _reply->set_status(status::SUCCESS);
        } else {
            _reply->set_status(status::UNKNOWN_ERROR);
        }
        return grpc::Status::OK;
    }

    grpc::Status PushData(
        grpc::ServerContext* _context, 
        const request* _request,
        reply* _reply
    ) override {
        bool found = (pool.find(_request->name_queue()) != pool.end());
        auto statuscode = (found == _request->has_node() == true) ? status::SUCCESS : status::UNKNOWN_ERROR;

        if(statuscode == status::SUCCESS){
            pool[_request->name_queue()]->enqueue(_request->node());
        }

        _reply->set_status(statuscode);
        return grpc::Status::OK;
    }

    grpc::Status PopData(
        grpc::ServerContext* _context, 
        const request* _request,
        reply* _reply
    ) override {
        bool found = (pool.find(_request->name_queue()) != pool.end());
        auto statuscode = status::UNKNOWN_ERROR;

        QueueStorageNode* reply_node;

        if(found){
            std::any node = pool[_request->name_queue()]->dequeue(true);

            if(node.has_value()){
                try {
                    QueueStorageNode* extracted_node = std::any_cast<queue_p_node>(node).get();
                    
                    reply_node = _reply->mutable_node();
                    reply_node->CopyFrom(*extracted_node);

                    statuscode = status::SUCCESS;
                } catch(std::exception& e){
                    std::cout << e.what() << std::endl;
                }
            }
        }
        
        _reply->set_status(statuscode);
        return grpc::Status::OK;
    }
};


class queue_server {
private:
    std::map<std::string, std::unique_ptr<ts_queue>> pool;
    std::unique_ptr<grpc::Server> server;
    std::unique_ptr<std::thread> thread_server; 

    bool is_running = false;
public:

    queue_server() = default;
    ~queue_server() = default;

    queue_server(const queue_server&) = delete;
    queue_server(queue_server&&) = delete;
    queue_server& operator=(queue_server&&) = delete;
    queue_server operator=(const queue_server&) = delete;

    void run(const std::string& address, uint16_t port){
        std::string server_address = absl::StrFormat("%s:%d", address, port);
        

        grpc::EnableDefaultHealthCheckService(true);
        //grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());


        QueueServiceImpl service(pool);
        builder.RegisterService(&service);

        server = builder.BuildAndStart();
        is_running = true;
        server->Wait();
    }

    void stop(){
        if(is_running){
            server->Shutdown();
            is_running = false;
        };
    }

};

};