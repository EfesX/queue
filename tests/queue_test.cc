#include "gtest/gtest.h"

#include <string>
#include <iostream>

//#include "queue.hpp"

//using namespace efesx;

TEST(queue_test, types_test)
{
    /*
    struct test_t {
        int a;
        bool b;
        float c;
    };

    int arr[3] = {8, 7, 6};

    queue q;
    q.enqueue(8, 0);
    q.enqueue(9, 0);
    q.enqueue(std::string{"string"}, 0);
    q.enqueue(53.5, 0);
    q.enqueue(true, 0);
    q.enqueue(test_t{3, 1, 2}, 0);
    q.enqueue(arr, 3, 0);

    int i;
    std::string s;
    double f;
    bool b;
    test_t st;
    int a[3];

    q.dequeue(i); ASSERT_EQ(i, 8);
    q.dequeue(i); ASSERT_EQ(i, 9);
    q.dequeue(s); ASSERT_EQ(s, "string");
    q.dequeue(f); EXPECT_TRUE((f < 53.55) && (f > 53.45));
    q.dequeue(b); EXPECT_TRUE(b);
    q.dequeue(st); 
        ASSERT_EQ(st.a, 3);
        ASSERT_EQ(st.b, 1);
        ASSERT_EQ(st.c, 2);
    q.dequeue(a);
        ASSERT_EQ(a[0], 8);
        ASSERT_EQ(a[1], 7);
        ASSERT_EQ(a[2], 6);
    */
}

TEST(queue_test, priority_test){
    /*
    queue q;

    q.enqueue(0, 0);
    q.enqueue(1, 0);
    q.enqueue(3, 5);
    q.enqueue(4, 3);
    q.enqueue(5, 125);
    q.enqueue(6, 254);
    q.enqueue(7, 254);
    q.enqueue(8, 254);
    q.enqueue(9, 255);
    q.enqueue(10, 255);
    q.enqueue(11, 255);
    q.enqueue(12, 0);

    int val;
    q.dequeue(val); ASSERT_EQ(val, 9);
    q.dequeue(val); ASSERT_EQ(val, 10);
    q.dequeue(val); ASSERT_EQ(val, 11);
    q.dequeue(val); ASSERT_EQ(val, 6);
    q.dequeue(val); ASSERT_EQ(val, 7);
    q.dequeue(val); ASSERT_EQ(val, 8);
    q.dequeue(val); ASSERT_EQ(val, 5);
    q.dequeue(val); ASSERT_EQ(val, 3);
    q.dequeue(val); ASSERT_EQ(val, 4);
    q.dequeue(val); ASSERT_EQ(val, 0);
    q.dequeue(val); ASSERT_EQ(val, 1);
    q.dequeue(val); ASSERT_EQ(val, 12);
    */
    
}
