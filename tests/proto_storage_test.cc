#include "gtest/gtest.h"

#include "detail/proto_storage.hpp"

using namespace efesx::queue::detail;

template<typename T>
void test_type(proto_storage& s, T&& _val){
    auto size_before = s.amount();

    s.insert(0, _val);

    auto test_1 = [](proto_storage& s, T& v){
        s.insert(0, v);
    };

    auto test_2 = [](proto_storage& s, const T& v){
        s.insert(0, v);
    };

    T val = _val;
    test_1(s, val);
    test_2(s, val);

    EXPECT_EQ(s.amount(), size_before + 3);
}

TEST(proto_storage_test, test_1)
{
    proto_storage s;

    test_type(s, int{8});
    test_type(s, int8_t{8});
    test_type(s, int16_t{8});
    test_type(s, int32_t{8});
    test_type(s, int64_t{8});

    test_type(s, uint8_t{8});
    test_type(s, uint16_t{8});
    test_type(s, uint32_t{8});
    test_type(s, uint64_t{8});

    test_type(s, bool{true});

    test_type(s, double{1.111111});
    test_type(s, float{2.2222222});

    test_type(s, std::string("asdasd"));

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
