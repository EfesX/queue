#include "gtest/gtest.h"

#include <string>
#include <iostream>

#include "detail/list_storage.hpp"

using namespace efesx::queue::detail;

TEST(list_storage_test, basic_test)
{
    list_storage ls;
    unsigned int arr[2] = { 0x12345678, 0xdeadcafe };
    ls.insert(100500);
    ls.insert("one", sizeof("one"));
    ls.insert(std::string("two"));
    ls.insert(2.3);
    ls.insert(arr, 8);

    EXPECT_EQ(ls.extract<int>().value_or(-1), 100500);
    
    EXPECT_EQ(ls.extract<float>().value_or(-1.0), -1.0);

    ls.pop();
    EXPECT_EQ(ls.extract<std::string>().value_or("-1"), std::string("one", sizeof("one")));
    ls.pop();
    EXPECT_EQ(ls.extract<std::string>().value_or("-1"), std::string("two"));
    ls.pop();
    EXPECT_TRUE((ls.extract<double>().value_or(-1.0) > 2.25) && (ls.extract<double>().value_or(-1.0) < 2.35));
    ls.pop();
    int res[2];
    ls.extract(res);
    EXPECT_EQ(arr[0], res[0]);
    EXPECT_EQ(arr[1], res[1]);
    ls.pop();

    EXPECT_EQ(ls.extract<int>().value_or(-1), -1);
}

TEST(list_storage_test, extract_types_test)
{
}

TEST(list_storage_test, extract_priority_test)
{
}
