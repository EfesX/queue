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
}
