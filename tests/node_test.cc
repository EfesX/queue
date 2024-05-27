#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include "detail/node.hpp"

using namespace efesx::queue::detail;

TEST(simple_test, node_test_1)
{
    {
        int val;
        node(5).restore(val);
        ASSERT_TRUE(val == 5);
    }
    {
        int val = 9;
        int res[1];
        node(&val, 1).restore(res);
        ASSERT_TRUE(res[0] == 9);
    }
    {
        bool val;
        node(true).restore(val);
        ASSERT_TRUE(val == true);
    }
    {
        char val;
        node('\n').restore(val);
        ASSERT_TRUE(val == '\n');
    }
    {
        std::string val;
        node(std::string("te\rst\n\0")).restore(val);
        ASSERT_TRUE(val == "te\rst\n\0");
    }
    {
        std::string val = "test";
        void* res = std::malloc(val.size());
        node(val.c_str(), val.size()).restore((char*)res);
        ASSERT_TRUE(std::string((char*)res) == val);
    }
    {
        char* val = (char*)std::malloc(sizeof("test"));
        node("test", sizeof("test")).restore(val);
        ASSERT_TRUE(std::string(val) == "test");
    }
    {
        int arr[3] = {1, 2, 3};
        int res[3];
        node(arr, 3).restore(res);
        ASSERT_TRUE(arr[0] == res[0]);
        ASSERT_TRUE(arr[1] == res[1]);
        ASSERT_TRUE(arr[2] == res[2]);
    }
    {   // @todo node must keep another nodes
        // kinda: node(node(node(5)));
    }
    {
        node(std::vector<int>{1, 2, 3});
    }
    {
        node(std::list<int>{1, 2, 3});
    }
    {
        node(std::set<int>{1, 2, 3});
    }
    {
        node(std::unordered_set<int>{1, 2, 3});
    }
    {
        //node(std::map<int, int>{{1, 2}});
    }
    {
        //node(std::unordered_map<int, int>{{1, 2}, {3, 4}, {5, 6}});
    }
}
