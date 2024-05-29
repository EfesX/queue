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
