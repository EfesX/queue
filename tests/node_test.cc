#include "gtest/gtest.h"

#include "detail/node.hpp"

#include <string>

using namespace detail;

TEST(node_test_1, node_test)
{
    int int_val;
    std::string str_val = "te\rst\n\0";
    struct test_t {
        int a;
        bool b;
        float c;
    };
    int arr[2] = {5, 25};
    std::string arr2[2] = {"hello", "world"};
    test_t arr3[2] = {
        test_t{0, true, 65.8},
        test_t{1, false, 70.3}
    };

    { // int
        node n = node(5);
        n.restore(int_val);
        ASSERT_EQ(int_val, 5);
    }
    { // string
        node n = node(str_val);
        str_val.clear();
        EXPECT_FALSE(str_val == "te\rst\n\0");
        n.restore(str_val);
        EXPECT_TRUE(str_val == "te\rst\n\0");
    }
    { // trivial struct
        node n = node(test_t{55, false, 88.5});
        test_t res;
        n.restore(res);
        ASSERT_EQ(res.a, 55);
        ASSERT_EQ(res.b, false);
        EXPECT_TRUE((res.c < 88.55) && ((res.c > 88.45)));
    }
    { // array
        {
            node n = node(arr, 2);
            int tarr[2];
            n.restore(tarr);
            ASSERT_EQ(tarr[0], 5);
            ASSERT_EQ(tarr[1], 25);
        }
        {
            node n = node(arr2, 2);
            std::string tarr[2];
            n.restore(tarr);
            ASSERT_EQ(tarr[0], "hello");
            ASSERT_EQ(tarr[1], "world");
        }
        {
            node n = node(arr3, 2);
            test_t tarr[2];
            n.restore(tarr);
            ASSERT_EQ(tarr[0].a, 0);
            ASSERT_EQ(tarr[0].b, true);
            EXPECT_TRUE((tarr[0].c > 65.75) && (tarr[0].c < 65.85));
            ASSERT_EQ(tarr[1].a, 1);
            ASSERT_EQ(tarr[1].b, false);
            EXPECT_TRUE((tarr[1].c > 70.25) && (tarr[0].c < 70.35));
        }
    }
}
