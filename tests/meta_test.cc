#include "gtest/gtest.h"

#include "detail/meta.hpp"

#include <string>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

using namespace efesx::queue::detail;

template<typename T>
bool test(T) requires (Container<T>) {
    return true;
}
template<typename T>
bool test(T) requires (!Container<T>) {
    return false;
}

TEST(meta_test, detector_test)
{
    ASSERT_TRUE(detector<int>::type == value_t::int_t);
    ASSERT_TRUE(detector<bool>::type == value_t::bool_t);
    ASSERT_TRUE(detector<std::string>::type == value_t::string_t);
    ASSERT_TRUE(detector<char>::type == value_t::char_t);

    ASSERT_TRUE(detector<int*>::type == value_t::null_t);
    ASSERT_TRUE(detector<bool*>::type == value_t::null_t);
    ASSERT_TRUE(detector<std::string*>::type == value_t::null_t);
    ASSERT_TRUE(detector<char*>::type == value_t::null_t);

    ASSERT_TRUE(detector<int&>::type == value_t::null_t);
    ASSERT_TRUE(detector<bool&>::type == value_t::null_t);
    ASSERT_TRUE(detector<std::string&>::type == value_t::null_t);
    ASSERT_TRUE(detector<char&>::type == value_t::null_t);
}

TEST(meta_test, container_test)
{
    ASSERT_TRUE(test(std::vector<int>{}));
    ASSERT_TRUE(test(std::vector<std::vector<int>>{}));
    ASSERT_TRUE(test(std::list<int>{}));
    ASSERT_TRUE(test(std::set<int>{}));
    ASSERT_TRUE(test(std::unordered_set<int>{}));
    ASSERT_TRUE(test(std::map<int, int>{}));
    ASSERT_TRUE(test(std::unordered_map<int, int>{}));

    ASSERT_FALSE(test(int{}));
    ASSERT_FALSE(test(char{}));
    ASSERT_FALSE(test(bool{}));
    ASSERT_FALSE(test(std::string{}));
}
