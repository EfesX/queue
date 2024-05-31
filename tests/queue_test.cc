#include "gtest/gtest.h"

#include <string>
#include <iostream>

#include "queue.hpp"
#include "utils.hpp"

using namespace efesx::queue;

TEST(QueueTest, Simple)
{
    queue meq;

    EXPECT_TRUE(meq.empty());
    EXPECT_EQ(meq.amount(), 0);

    int int_vars[5] = {-2, -1, 0, 1, 2};
    meq.enqueue(0, int_vars[0]);
    meq.enqueue(1, int_vars[1]);
    meq.enqueue(2, int_vars[2]);
    meq.enqueue(3, int_vars[3]);
    meq.enqueue(4, int_vars[4]);
    meq.enqueue(0, int_vars, sizeof(int_vars));

    EXPECT_FALSE(meq.empty());
    EXPECT_EQ(meq.amount(), 6);

    EXPECT_EQ(meq.dequeue()->data_case(), node_value_t::kSint32Value);
    EXPECT_TRUE(meq.dequeue()->has_sint32_value());
    EXPECT_EQ(meq.dequeue()->sint32_value(), int_vars[4]);
    meq.pop();

    EXPECT_EQ(meq.dequeue()->sint32_value(),  int_vars[3]); meq.pop();
    EXPECT_EQ(meq.dequeue()->sint32_value(),  int_vars[2]); meq.pop();
    EXPECT_EQ(meq.dequeue()->sint32_value(),  int_vars[1]); meq.pop();
    EXPECT_EQ(meq.dequeue()->sint32_value(),  int_vars[0]); meq.pop();

    EXPECT_EQ(meq.dequeue()->data_case(), node_value_t::kRawData);
    EXPECT_TRUE(meq.dequeue()->has_raw_data());
    EXPECT_EQ(queue_raw_data_cast<int>(meq.dequeue()->raw_data())[0], -2);
    meq.pop();

    EXPECT_TRUE(meq.empty());
    EXPECT_EQ(meq.amount(), 0);

    meq.enqueue(0, int8_t{-56});
    EXPECT_EQ(meq.dequeue()->sint32_value(), -56);
}

TEST(QueueTest, SaveLoad)
{
    queue meq;
    meq.enqueue(0, std::string("asda23sd"));
    meq.enqueue(0, std::string("asd2asd"));
    meq.enqueue(0, std::string("asdgasd"));
    meq.enqueue(0, std::string("asdasdfasd"));
    meq.enqueue(0, std::string("asdasdasd"));
    meq.enqueue(0, std::string("asdasd"));
    meq.enqueue(0, std::string("asda.csd"));
    meq.enqueue(0, std::string("asda54sd"));
    meq.enqueue(0, std::string("assfdgddasd"));
    meq.enqueue(0, std::string("asgagdasd"));
    meq.enqueue(0, std::string("asdl.asd"));
    meq.enqueue(0, std::string("asdahfsd"));

    EXPECT_EQ(meq.amount(), 12);
    meq.save_to_disk("equeue.data");

    queue meqq;
    meqq.load_from_disk("equeue.data");
    EXPECT_EQ(meq, meqq);
}

TEST(QueueTest, ManyData){
    const std::size_t N_DATA = 1000000;

    queue meq;
    for (auto i = 0; i < N_DATA; i++){
        meq.enqueue(0, i);
    }
    EXPECT_EQ(meq.amount(), N_DATA);
    meq.save_to_disk("equeue_md.data");

    queue meqq;
    meqq.load_from_disk("equeue_md.data");
    EXPECT_EQ(meq, meqq);
}