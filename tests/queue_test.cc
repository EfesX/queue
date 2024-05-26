#include "gtest/gtest.h"

#include "queue.hpp"

#include <string>

using namespace efesx;

TEST(queue_test_1, queue_test)
{
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
        
}
