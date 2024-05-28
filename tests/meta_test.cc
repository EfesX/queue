#include "gtest/gtest.h"

#include "detail/meta.hpp"

using namespace efesx::queue::detail;

TEST(meta_test, is_integral_concept_test)
{
    EXPECT_TRUE(IsIntegral<bool>);
    EXPECT_TRUE(IsIntegral<char>);
    EXPECT_TRUE(IsIntegral<int8_t>);
    EXPECT_TRUE(IsIntegral<int16_t>);
    EXPECT_TRUE(IsIntegral<int32_t>);
    EXPECT_TRUE(IsIntegral<int64_t>);
    EXPECT_TRUE(IsIntegral<int>);
    EXPECT_TRUE(IsIntegral<uint8_t>);
    EXPECT_TRUE(IsIntegral<uint16_t>);
    EXPECT_TRUE(IsIntegral<uint32_t>);
    EXPECT_TRUE(IsIntegral<uint64_t>);
    EXPECT_TRUE(IsIntegral<unsigned>);
    EXPECT_TRUE(IsIntegral<unsigned long>);
    
    EXPECT_FALSE(IsIntegral<int&>);
    EXPECT_FALSE(IsIntegral<int&&>);
    EXPECT_FALSE(IsIntegral<int*>);
    EXPECT_FALSE(IsIntegral<int**>);

    EXPECT_FALSE(IsIntegral<double>);
    EXPECT_FALSE(IsIntegral<float>);

    EXPECT_FALSE(IsIntegral<double&>);
    EXPECT_FALSE(IsIntegral<float*>);

    EXPECT_FALSE(IsIntegral<std::string>);
    EXPECT_FALSE(IsIntegral<std::string&>);
    EXPECT_FALSE(IsIntegral<std::string*>);
}

TEST(meta_test, is_floating_concept_test)
{
    EXPECT_FALSE(IsFloating<bool>);
    EXPECT_FALSE(IsFloating<char>);
    EXPECT_FALSE(IsFloating<int8_t>);
    EXPECT_FALSE(IsFloating<int16_t>);
    EXPECT_FALSE(IsFloating<int32_t>);
    EXPECT_FALSE(IsFloating<int64_t>);
    EXPECT_FALSE(IsFloating<int>);
    EXPECT_FALSE(IsFloating<uint8_t>);
    EXPECT_FALSE(IsFloating<uint16_t>);
    EXPECT_FALSE(IsFloating<uint32_t>);
    EXPECT_FALSE(IsFloating<uint64_t>);
    EXPECT_FALSE(IsFloating<unsigned>);
    EXPECT_FALSE(IsFloating<unsigned long>);
    
    EXPECT_FALSE(IsFloating<int&>);
    EXPECT_FALSE(IsFloating<int&&>);
    EXPECT_FALSE(IsFloating<int*>);
    EXPECT_FALSE(IsFloating<int**>);

    EXPECT_TRUE(IsFloating<double>);
    EXPECT_TRUE(IsFloating<float>);

    EXPECT_FALSE(IsFloating<double&>);
    EXPECT_FALSE(IsFloating<float*>);

    EXPECT_FALSE(IsFloating<std::string>);
    EXPECT_FALSE(IsFloating<std::string&>);
    EXPECT_FALSE(IsFloating<std::string*>);
}

TEST(meta_test, is_string_concept_test)
{
    EXPECT_FALSE(IsString<bool>);
    EXPECT_FALSE(IsString<char>);
    EXPECT_FALSE(IsString<int8_t>);
    EXPECT_FALSE(IsString<int16_t>);
    EXPECT_FALSE(IsString<int32_t>);
    EXPECT_FALSE(IsString<int64_t>);
    EXPECT_FALSE(IsString<int>);
    EXPECT_FALSE(IsString<uint8_t>);
    EXPECT_FALSE(IsString<uint16_t>);
    EXPECT_FALSE(IsString<uint32_t>);
    EXPECT_FALSE(IsString<uint64_t>);
    EXPECT_FALSE(IsString<unsigned>);
    EXPECT_FALSE(IsString<unsigned long>);
    
    EXPECT_FALSE(IsString<int&>);
    EXPECT_FALSE(IsString<int&&>);
    EXPECT_FALSE(IsString<int*>);
    EXPECT_FALSE(IsString<int**>);

    EXPECT_FALSE(IsString<double>);
    EXPECT_FALSE(IsString<float>);

    EXPECT_FALSE(IsString<double&>);
    EXPECT_FALSE(IsString<float*>);

    EXPECT_TRUE(IsString<std::string>);
    EXPECT_TRUE(IsString<std::string&>);
    EXPECT_FALSE(IsString<std::string*>);
}
