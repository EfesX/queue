#include "gtest/gtest.h"

<<<<<<< HEAD
#include "web/queue_server.hpp"
#include "web/queue_client.hpp"
#include "ts_queue.hpp"
=======
#include "equeue/web/queue_server.hpp"
#include "equeue/web/queue_client.hpp"
#include "equeue/ts_queue.hpp"
>>>>>>> cmake_refactoring

using namespace efesx::queue::web;
using namespace efesx::ts_queue;

TEST(QueueWebTest, Simple)
{
    queue_server server;
    std::thread th([&](){
        server.run("0.0.0.0", 55555);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    queue_client client("0.0.0.0", 55555);

    EXPECT_EQ(client.create_queue("q1"), status::SUCCESS);
    EXPECT_EQ(client.create_queue("q1"), status::UNKNOWN_ERROR);

    EXPECT_EQ(client.create_queue("q2"), status::SUCCESS);
    EXPECT_EQ(client.create_queue("q3"), status::SUCCESS);

    EXPECT_EQ(client.delete_queue("q3"), status::SUCCESS);
    EXPECT_EQ(client.delete_queue("q3"), status::UNKNOWN_ERROR);

    {
        node* _node = new node();
        _node->set_priority(666);
        _node->set_sint32_value(56);
        EXPECT_EQ(client.push_data("q1", _node), status::SUCCESS);
    }
    {
        std::shared_ptr<node> _node;
        EXPECT_EQ(client.pop_data("q1", _node), status::SUCCESS);
        EXPECT_EQ(_node->priority(), 666);
        EXPECT_EQ(_node->sint32_value(), 56);
    }
    {
        std::shared_ptr<node> _node;
        EXPECT_EQ(client.pop_data("q1", _node), status::UNKNOWN_ERROR);
    }
    {
        node* _node = new node();
        _node->set_priority(1);
        _node->set_string_value("test q1");
        EXPECT_EQ(client.push_data("q1", _node), status::SUCCESS);
    }
    {
        node* _node = new node();
        _node->set_priority(1);
        _node->set_string_value("test q2");
        EXPECT_EQ(client.push_data("q2", _node), status::SUCCESS);
    }
    
    {
        std::shared_ptr<node> _node;
        EXPECT_EQ(client.pop_data("q1", _node), status::SUCCESS);
        EXPECT_EQ(_node->priority(), 1);
        EXPECT_EQ(_node->string_value(), "test q1");
    }
    {
        std::shared_ptr<node> _node;
        EXPECT_EQ(client.pop_data("q1", _node), status::UNKNOWN_ERROR);
    }
    {
        std::shared_ptr<node> _node;
        EXPECT_EQ(client.pop_data("q2", _node), status::SUCCESS);
        EXPECT_EQ(_node->priority(), 1);
        EXPECT_EQ(_node->string_value(), "test q2");
    }
    {
        std::shared_ptr<node> _node;
        EXPECT_EQ(client.pop_data("q2", _node), status::UNKNOWN_ERROR);
    }
    {
        std::shared_ptr<node> _node;
        EXPECT_EQ(client.pop_data("unexisted", _node), status::UNKNOWN_ERROR);
    }

    server.stop();
    th.join();
}


class Fixture : public ::testing::TestWithParam<int>{};

TEST_P(Fixture, ManyClients){
    const int NUM_DATA = 1000;
    const int NUM_CLIENTS = GetParam() + 1;

    queue_server server;
    std::thread th([&](){
        server.run("0.0.0.0", 55555);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    std::vector<std::thread> clients;

    for (auto i = 0; i < NUM_CLIENTS; i++){
        clients.push_back(std::thread([=](){
            queue_client client("0.0.0.0", 55555);

            // someone one will create queue successfully
            client.create_queue("queue");

            for (auto j = 0; j < NUM_DATA; j++){
                node* _node = new node();
                _node->set_priority((i * NUM_DATA) + j);
                _node->set_uint32_value(j);

                client.push_data("queue", _node);

                std::this_thread::yield();
            }
        }));
    }

    for (auto i = 0; i < NUM_CLIENTS; i++){
        clients[i].join();
    }

    queue_client client("0.0.0.0", 55555);

    EXPECT_EQ(client.create_queue("queue"), status::UNKNOWN_ERROR);

    for (auto i = 0; i < NUM_CLIENTS; i++){
        for (auto j = 0; j < NUM_DATA; j++){
            std::shared_ptr<node> _node;
            EXPECT_EQ(client.pop_data("queue", _node), status::SUCCESS);
            EXPECT_EQ(_node->priority(), (NUM_CLIENTS * NUM_DATA) - 1 - i * NUM_DATA - j);
        }
    }

    std::shared_ptr<node> _node;
    EXPECT_EQ(client.pop_data("queue", _node), status::UNKNOWN_ERROR);

    server.stop();
    th.join();
}

INSTANTIATE_TEST_CASE_P(QueueWebTest, Fixture, ::testing::Range(1, 6));