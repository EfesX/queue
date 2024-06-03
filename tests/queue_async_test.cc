#include "gtest/gtest.h"

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>

#include "queue_async.hpp"
#include "utils.hpp"

#include <thread>

using namespace efesx::queue_async;

TEST(QueueAsyncTest, Simple)
{
    queue_async queue;
    queue.enqueue(0, 5);
    queue.enqueue(0, -25);
    queue.enqueue(0, 65.8);
    queue.enqueue(0, "test", sizeof("test"));

    EXPECT_EQ(queue.amount(), 4);

    queue.save_to_disk("asynctest.simple.data");

    queue_async r_queue;
    r_queue.load_from_disk("asynctest.simple.data");

    EXPECT_EQ(queue, r_queue);
}

class Fixture : public ::testing::TestWithParam<int>{};

TEST_P(Fixture, SPSC){
    const int NUM_DATA = 100000;

    queue_async ref_queue;
    queue_async p_queue;

    {
        std::thread producer([&](){
            for(auto i = 0; i < NUM_DATA; i++){
                int32_t val = std::rand();
                p_queue.enqueue(0, val);
                ref_queue.enqueue(0, val);
                std::this_thread::yield();
            }
        });

        queue_async c_queue;

        std::thread consumer([&](){
            for(auto i = 0; i < NUM_DATA; i++){
                while(p_queue.amount() == 0){std::this_thread::yield();}
                std::any val = p_queue.dequeue();
                c_queue.enqueue(0, std::any_cast<node_p_t>(val)->sint32_value());
                p_queue.pop();
            }
        });

        producer.join();
        consumer.join();

        c_queue.save_to_disk("asynctest.spsc.data");

        EXPECT_EQ(ref_queue, c_queue);
    }
    {
        queue_async queue;
        queue.load_from_disk("asynctest.spsc.data");

        EXPECT_EQ(ref_queue, queue);
    }
    
}

TEST_P(Fixture, MPMC){
    const int NUM_DATA = 1000;
    const int NUM_CONSUMERS = 4;
    const int NUM_PRODUCERS = 4;

    queue_async p_queue;
    queue_async c_queue;
    queue_async ref_queue;

    std::vector<std::thread> consumers;
    std::vector<std::thread> producers;

    for (auto i = 0; i < NUM_CONSUMERS; i++){
        consumers.emplace_back([&](){
            while(c_queue.amount() != (NUM_DATA * NUM_PRODUCERS)){

                auto val = p_queue.dequeue(true);
                if (val.has_value()){
                    c_queue.enqueue(
                        std::any_cast<node_p_t>(val)->priority(), 
                        std::any_cast<node_p_t>(val)->sint32_value()
                    );
                }

                std::this_thread::yield();
            }
        });
    }

    std::this_thread::sleep_for(std::chrono::microseconds(3));;

    for (auto i = 0; i < NUM_PRODUCERS; i++){
        producers.emplace_back([&](){
            for(auto i = 0; i < NUM_DATA; i++){
                int32_t val = std::rand();
                p_queue.enqueue(i, val);
                ref_queue.enqueue(i, val);
                std::this_thread::yield();
            }
        });
    }

    for (auto i = 0; i < NUM_PRODUCERS; i++){
        producers[i].join();
    }
    for (auto i = 0; i < NUM_CONSUMERS; i++){
        consumers[i].join();
    }

    EXPECT_EQ(p_queue.amount(), 0);
    EXPECT_TRUE(p_queue.empty());
    EXPECT_EQ(c_queue.amount(), (NUM_DATA * NUM_PRODUCERS));
    EXPECT_EQ(c_queue.amount(), ref_queue.amount());

    EXPECT_EQ(ref_queue, c_queue);

    c_queue.save_to_disk("asynctest.mpmc.data");

    queue_async r_queue;
    r_queue.load_from_disk("asynctest.mpmc.data");
    EXPECT_EQ(r_queue, c_queue);
    

}

INSTANTIATE_TEST_CASE_P(QueueAsyncTest, Fixture, ::testing::Range(1, 11));
