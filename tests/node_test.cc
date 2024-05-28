#include "gtest/gtest.h"

#include <string>

#include "detail/node.hpp"

using namespace efesx::queue::detail;

TEST(simple_test, node_test_1)
{
    {
        node n;
        ASSERT_TRUE(n.size() == 0);
        ASSERT_TRUE(n.has_value() == false);
        ASSERT_TRUE(n.value_type() == value_t::null);
    }
    {
        node n(54);
        ASSERT_TRUE(n.size() == 4);
        ASSERT_TRUE(n.has_value() == true);
        ASSERT_TRUE(n.value_type() == value_t::integer);
        ASSERT_TRUE(n.value_cast<int>() == 54);
    }
    {
        node n(0x01234567);
        ASSERT_TRUE(n.size() == 4);
        ASSERT_TRUE(n.has_value() == true);
        ASSERT_TRUE(n.value_type() == value_t::integer);
        ASSERT_TRUE(n.value_cast<uint8_t>() == 0x67);
    }
    {
        node n(double{6.55});
        ASSERT_TRUE(n.size() == 8);
        ASSERT_TRUE(n.has_value() == true);
        ASSERT_TRUE(n.value_type() == value_t::floating);
        ASSERT_TRUE((n.value_cast<double>() > 6.50) && (n.value_cast<double>() < 6.60));
    }
    {
        node n(float{6.55});
        ASSERT_TRUE(n.size() == 4);
        ASSERT_TRUE(n.has_value() == true);
        ASSERT_TRUE(n.value_type() == value_t::floating);
        ASSERT_TRUE((n.value_cast<float>() > 6.50) && (n.value_cast<float>() < 6.60));
    }
    {
        node n(std::string{"test"});
        EXPECT_EQ(n.size(), 4);
        ASSERT_TRUE(n.has_value() == true);
        ASSERT_TRUE(n.value_type() == value_t::text);
        ASSERT_TRUE(n.value_cast<std::string>() == "test");
    }
    {
        node n("test", sizeof("test"));
        EXPECT_EQ(n.size(), 5);
        ASSERT_TRUE(n.has_value() == true);
        ASSERT_TRUE(n.value_type() == value_t::text);
        EXPECT_EQ(std::string("test", sizeof("test")), n.value_cast<std::string>());
    }
    {
        try {
            node n(666);
            EXPECT_EQ(4, n.size());
            EXPECT_EQ(true, n.has_value());
            EXPECT_EQ(value_t::integer, n.value_type());
            n.value_cast<std::string>();
        } catch (std::runtime_error& e){
            EXPECT_STREQ("Bad Node Value Cast", e.what());
        }
    }
    {
        try {
            node n;
            EXPECT_EQ(n.size(), 0);
            EXPECT_EQ(false, n.has_value());
            EXPECT_EQ(value_t::null, n.value_type());
            n.value_cast<int>();
        } catch (std::runtime_error& e){
            EXPECT_STREQ("Node Value Is Empty", e.what());
        }
    }
    {
        int arr[3] = {0, 1, 2};
        node n(arr, 12);
        EXPECT_EQ(12, n.size());
        EXPECT_EQ(true, n.has_value());
        EXPECT_EQ(value_t::blob, n.value_type());
        int* res = static_cast<int*>(std::malloc(sizeof(int) * n.size()));
        n.blob_copy(res);
        EXPECT_EQ(arr[0], res[0]);
        EXPECT_EQ(arr[1], res[1]);
        EXPECT_EQ(arr[2], res[2]);
        std::free(res);
    }

}
