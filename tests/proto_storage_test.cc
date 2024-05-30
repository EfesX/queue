#include "gtest/gtest.h"

#include "detail/proto_storage.hpp"

using namespace efesx::queue::detail;

TEST(proto_storage_test, test_1)
{
    proto_storage s;

    s.insert(1, int{8});
    s.insert(0, int8_t{8});
    s.insert(0, int16_t{8});
    s.insert(0, int32_t{8});
    s.insert(0, int64_t{8});

    s.insert(0, uint8_t{8});
    s.insert(0, uint16_t{8});
    s.insert(0, uint32_t{8});
    s.insert(0, uint64_t{8});

    s.insert(0, bool{false});
    s.insert(0, bool{true});

    s.insert(0, std::string("string"));

    s.insert(0, "new string", sizeof("new string"));

    s.save("test.pb");

    proto_storage ss;
    ss.load("test.pb");

    EXPECT_EQ(s, ss);

    ss.insert(0, uint32_t{8});

    EXPECT_TRUE(s != ss);
}

TEST(proto_storage_test, test_2)
{
    proto_storage s;

    EXPECT_TRUE(s.empty());

    s.insert(1, int{-8});
    s.insert(5, uint64_t{1});

    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.amount(), 2);

    s.extract()->set_uint64_value(uint64_t{2});

    EXPECT_EQ(s.amount(), 2);
    EXPECT_EQ(s.extract()->uint64_value(), uint64_t{2});    
    
    s.pop();
    EXPECT_EQ(s.amount(), 1);

    EXPECT_EQ(s.extract()->sint32_value(), int{-8});
    EXPECT_EQ(s.extract()->sint32_value(), int{-8});
    EXPECT_EQ(s.amount(), 1);

    s.pop();
    EXPECT_EQ(s.amount(), 0);
    EXPECT_TRUE(s.empty());

    s.insert(0, std::string("one"));
    s.insert(1, uint16_t{2});
    s.insert(2, int64_t{-3000});

    EXPECT_EQ(s.extract()->sint64_value(), int64_t{-3000});
    s.pop();
    EXPECT_EQ(s.extract()->uint32_value(), uint16_t{2});
    s.pop();
    EXPECT_EQ(s.extract()->string_value(), std::string{"one"});
    s.pop();

    EXPECT_TRUE(s.empty());

    int arr[2] = { -6, -9 };

    s.insert(0, (void*)arr, 8);
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(((int*)s.extract()->raw_data().c_str())[0], -6);
    EXPECT_EQ(((int*)s.extract()->raw_data().c_str())[1], -9);
}
