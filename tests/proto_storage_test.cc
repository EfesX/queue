#include "gtest/gtest.h"

#include "equeue/detail/proto_storage.hpp"

using namespace efesx::queue::detail;

template<typename T>
void test_type(proto_storage& mps, T&& _val){
    auto size_before = mps.amount();

    mps.insert(0, _val);

    auto test_1 = [](proto_storage& mps, T& v){
        mps.insert(0, v);
    };

    auto test_2 = [](proto_storage& mps, const T& v){
        mps.insert(0, v);
    };

    T val = _val;
    test_1(mps, val);
    test_2(mps, val);

    EXPECT_EQ(mps.amount(), size_before + 3);
}

TEST(ProtoStorageTest, test_1)
{
    proto_storage mps;

    test_type(mps, int{8});
    test_type(mps, int8_t{8});
    test_type(mps, int16_t{8});
    test_type(mps, int32_t{8});
    test_type(mps, int64_t{8});

    test_type(mps, uint8_t{8});
    test_type(mps, uint16_t{8});
    test_type(mps, uint32_t{8});
    test_type(mps, uint64_t{8});

    test_type(mps, bool{true});

    test_type(mps, double{1.111111});
    test_type(mps, float{2.2222222});

    test_type(mps, std::string("asdasd"));

    mps.insert(0, "new string", sizeof("new string"));

    mps.save("test.pb");

    proto_storage mpss;
    mpss.load("test.pb");

    EXPECT_EQ(mps, mpss);

    mpss.insert(0, uint32_t{8});

    EXPECT_TRUE(mps != mpss);
}

TEST(ProtoStorageTest, test_2)
{
    proto_storage mps;

    EXPECT_TRUE(mps.empty());

    mps.insert(1, int{-8});
    mps.insert(5, uint64_t{1});

    EXPECT_FALSE(mps.empty());
    EXPECT_EQ(mps.amount(), 2);

    mps.extract()->set_uint64_value(uint64_t{2});

    EXPECT_EQ(mps.amount(), 2);
    EXPECT_EQ(mps.extract()->uint64_value(), uint64_t{2});    
    
    mps.pop();
    EXPECT_EQ(mps.amount(), 1);

    EXPECT_EQ(mps.extract()->sint32_value(), int{-8});
    EXPECT_EQ(mps.extract()->sint32_value(), int{-8});
    EXPECT_EQ(mps.amount(), 1);

    mps.pop();
    EXPECT_EQ(mps.amount(), 0);
    EXPECT_TRUE(mps.empty());

    mps.insert(0, std::string("one"));
    mps.insert(1, uint16_t{2});
    mps.insert(2, int64_t{-3000});

    EXPECT_EQ(mps.extract()->sint64_value(), int64_t{-3000});
    mps.pop();
    EXPECT_EQ(mps.extract()->uint32_value(), uint16_t{2});
    mps.pop();
    EXPECT_EQ(mps.extract()->string_value(), std::string{"one"});
    mps.pop();

    EXPECT_TRUE(mps.empty());

    int arr[2] = { -6, -9 };

    mps.insert(0, static_cast<void*>(arr), 8);
    EXPECT_FALSE(mps.empty());
    EXPECT_EQ((reinterpret_cast<const int*>(mps.extract()->raw_data().c_str()))[0], -6);
    EXPECT_EQ((reinterpret_cast<const int*>(mps.extract()->raw_data().c_str()))[1], -9);
}
