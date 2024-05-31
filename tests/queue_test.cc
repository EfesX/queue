#include "gtest/gtest.h"

#include <string>
#include <iostream>

#include "queue.hpp"
#include "utils.hpp"

using namespace efesx::queue;

TEST(queue_test, simple_test)
{
    queue q;

    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.amount(), 0);

    int int_vars[5] = {-2, -1, 0, 1, 2};
    q.enqueue(0, int_vars[0]);
    q.enqueue(1, int_vars[1]);
    q.enqueue(2, int_vars[2]);
    q.enqueue(3, int_vars[3]);
    q.enqueue(4, int_vars[4]);
    q.enqueue(0, int_vars, sizeof(int_vars));

    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.amount(), 6);

    EXPECT_EQ(q.dequeue()->data_case(), node_value_t::kSint32Value);
    EXPECT_TRUE(q.dequeue()->has_sint32_value());
    EXPECT_EQ(q.dequeue()->sint32_value(), int_vars[4]);
    q.pop();

    EXPECT_EQ(q.dequeue()->sint32_value(),  int_vars[3]); q.pop();
    EXPECT_EQ(q.dequeue()->sint32_value(),  int_vars[2]); q.pop();
    EXPECT_EQ(q.dequeue()->sint32_value(),  int_vars[1]); q.pop();
    EXPECT_EQ(q.dequeue()->sint32_value(),  int_vars[0]); q.pop();

    EXPECT_EQ(q.dequeue()->data_case(), node_value_t::kRawData);
    EXPECT_TRUE(q.dequeue()->has_raw_data());
    EXPECT_EQ(queue_raw_data_cast<int>(q.dequeue()->raw_data())[0], -2);
    q.pop();

    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.amount(), 0);

    q.enqueue(0, int8_t{-56});
    EXPECT_EQ(q.dequeue()->sint32_value(), -56);
}

TEST(queue_test, save_load_test)
{
    queue q;
    q.enqueue(0, std::string("asda23sd"));
    q.enqueue(0, std::string("asd2asd"));
    q.enqueue(0, std::string("asdgasd"));
    q.enqueue(0, std::string("asdasdfasd"));
    q.enqueue(0, std::string("asdasdasd"));
    q.enqueue(0, std::string("asdasd"));
    q.enqueue(0, std::string("asda.csd"));
    q.enqueue(0, std::string("asda54sd"));
    q.enqueue(0, std::string("assfdgddasd"));
    q.enqueue(0, std::string("asgagdasd"));
    q.enqueue(0, std::string("asdl.asd"));
    q.enqueue(0, std::string("asdahfsd"));

    EXPECT_EQ(q.amount(), 12);
    q.save_to_disk("equeue.data");

    queue qq;
    qq.load_from_disk("equeue.data");
    EXPECT_EQ(q, qq);
}

TEST(queue_test, many_data_test){
    std::size_t N_DATA = 1000000;

    queue q;
    for (auto i = 0; i < N_DATA; i++){
        q.enqueue(0, i);
    }
    EXPECT_EQ(q.amount(), N_DATA);
    q.save_to_disk("equeue_md.data");

    queue qq;
    qq.load_from_disk("equeue_md.data");
    EXPECT_EQ(q, qq);
}