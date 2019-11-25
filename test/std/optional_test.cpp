#include <gtest/gtest.h>

#include "../test_utils.h"
#include "optional_test_wrappers.h"
#include "optional_test_data.h"

using namespace std;
using namespace smart_block;

TEST(Optional_TestCase, Simple1)
{
    builder<uint32_t, optional<uint32_t>, uint32_t> builder;

    EXPECT_EQ(16, sizeof(builder)); // 4 + (4 + 1 + padding3) + 4

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(9, block.length()); // 1 + 4 + 0 + 4
    EXPECT_EQ(nullopt, block.get<1>());

    EXPECT_EQ(OptionalSimple1Str, to_string(block));
    EXPECT_EQ(OptionalSimple1Data, block.span());

    builder.set<0>(0x1111);
    builder.set<1>(0x2222);
    builder.set<2>(0x3333);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(13, block.length()); // 1 + 4 + 4 + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ(0x2222, block.get<1>());
    EXPECT_EQ(0x3333, block.get<2>());

    EXPECT_EQ(OptionalSimple1Str2, to_string(block));
    EXPECT_EQ(OptionalSimple1Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second);
    EXPECT_EQ(0x3333, third);
#   endif
}

TEST(Optional_TestCase, Simple2)
{
    builder<string, optional<uint32_t>, string> builder;

    EXPECT_EQ(72, sizeof(builder)); // 32 + (4 + 1 + padding3) + 32

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(5, block.length()); // 3 + (0 + 1) + 0 + (0 + 1)
    EXPECT_EQ("", block.get<0>());
    EXPECT_EQ(nullopt, block.get<1>());
    EXPECT_EQ("", block.get<2>());

    EXPECT_EQ(OptionalSimple2Str, to_string(block));
    EXPECT_EQ(OptionalSimple2Data, block.span());

    builder.set<0>("abcdefghijklm");
    builder.set<1>(0x1111);
    builder.set<2>("nopqrstuvwxyz");

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(35, block.length()); // 3 + (13 + 1) + 4 + (13 + 1)
    EXPECT_EQ("abcdefghijklm", block.get<0>());
    EXPECT_EQ(0x1111, block.get<1>());
    EXPECT_EQ("nopqrstuvwxyz", block.get<2>());

    EXPECT_EQ(OptionalSimple2Str2, to_string(block));
    EXPECT_EQ(OptionalSimple2Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(0x1111, second);
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(Optional_TestCase, Simple3)
{
    builder<uint32_t, optional<string>, uint32_t> builder;

    EXPECT_EQ(48, sizeof(builder)); // 4 + (32 + 1 + padding7) + 4

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(9, block.length()); // 1 + 4 + 0 + 4
    EXPECT_EQ(0, block.get<0>());
    EXPECT_EQ(nullopt, block.get<1>());
    EXPECT_EQ(0, block.get<2>());

    EXPECT_EQ(OptionalSimple3Str, to_string(block));
    EXPECT_EQ(OptionalSimple3Data, block.span());

    builder.set<0>(0x1111);
    builder.set<1>("abcdefghijklmnopqrstuvwxyz");
    builder.set<2>(0x2222);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(36, block.length()); // 1 + 4 + (26 + 1) + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.get<1>());
    EXPECT_EQ(0x2222, block.get<2>());

    EXPECT_EQ(OptionalSimple3Str2, to_string(block));
    EXPECT_EQ(OptionalSimple3Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", second);
    EXPECT_EQ(0x2222, third);
#   endif
}

TEST(Optional_TestCase, Complex1)
{
    builder<uint32_t, optional<block<uint32_t, uint32_t>>, uint32_t> builder;

    EXPECT_EQ(17, sizeof(builder)); // 4 + ((4 + 4) + 1) + 4

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(9, block.length()); // 1 + 4 + 0 + 4
    EXPECT_EQ(false, block.get<1>().has_value());

    EXPECT_EQ(OptionalComplex1Str, to_string(block));
    EXPECT_EQ(OptionalComplex1Data, block.span());

    builder.set<0>(0x1111);
    builder.sub<1>().set<0>(0x2222);
    builder.sub<1>().set<1>(0x3333);
    builder.set<2>(0x4444);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(17, block.length()); // 1 + 4 + (4 + 4) + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ(true, block.get<1>().has_value());
    EXPECT_EQ(0x2222, block.get<1>().value().get<0>());
    EXPECT_EQ(0x3333, block.get<1>().value().get<1>());
    EXPECT_EQ(0x4444, block.get<2>());

    EXPECT_EQ(OptionalComplex1Str2, to_string(block));
    EXPECT_EQ(OptionalComplex1Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(true, second.has_value());
    EXPECT_EQ(0x2222, second.value().get<0>());
    EXPECT_EQ(0x3333, second.value().get<1>());
    EXPECT_EQ(0x4444, third);
#   endif
}

TEST(Optional_TestCase, Complex2)
{
    builder<string, optional<block<uint32_t, uint32_t>>, string> builder;

    EXPECT_EQ(73, sizeof(builder)); // 32 + ((4 + 4) + 1) + 32

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(5, block.length()); // 3 + (0 + 1) + 0 + (0 + 1)
    EXPECT_EQ("", block.get<0>());
    EXPECT_EQ(false, block.get<1>().has_value());
    EXPECT_EQ("", block.get<2>());

    EXPECT_EQ(OptionalComplex2Str, to_string(block));
    EXPECT_EQ(OptionalComplex2Data, block.span());

    builder.set<0>("abcdefghijklm");
    builder.sub<1>().set<0>(0x1111);
    builder.sub<1>().set<1>(0x2222);
    builder.set<2>("nopqrstuvwxyz");

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(39, block.length()); // 3 + (13 + 1) + (4 + 4) + (13 + 1)
    EXPECT_EQ("abcdefghijklm", block.get<0>());
    EXPECT_EQ(true, block.get<1>().has_value());
    EXPECT_EQ(0x1111, block.get<1>().value().get<0>());
    EXPECT_EQ(0x2222, block.get<1>().value().get<1>());
    EXPECT_EQ("nopqrstuvwxyz", block.get<2>());

    EXPECT_EQ(OptionalComplex2Str2, to_string(block));
    EXPECT_EQ(OptionalComplex2Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(true, second.has_value());
    EXPECT_EQ(0x1111, second.value().get<0>());
    EXPECT_EQ(0x2222, second.value().get<1>());
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(Optional_TestCase, Complex3)
{
    builder<uint32_t, optional<block<string, string>>, uint32_t> builder;

    EXPECT_EQ(73, sizeof(builder)); // 4 + ((32 + 32) + 1) + 4

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(9, block.length()); // 1 + 4 + 0 + 4
    EXPECT_EQ(0, block.get<0>());
    EXPECT_EQ(false, block.get<1>().has_value());
    EXPECT_EQ(0, block.get<2>());

    EXPECT_EQ(OptionalComplex3Str, to_string(block));
    EXPECT_EQ(OptionalComplex3Data, block.span());

    builder.set<0>(0x1111);
    builder.sub<1>().set<0>("abcdefghijklm");
    builder.sub<1>().set<1>("nopqrstuvwxyz");
    builder.set<2>(0x2222);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(39, block.length()); // 1 + 4 + (2 + (13 + 1) + (13 + 1)) + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ(true, block.get<1>().has_value());
    EXPECT_EQ("abcdefghijklm", block.get<1>().value().get<0>());
    EXPECT_EQ("nopqrstuvwxyz", block.get<1>().value().get<1>());
    EXPECT_EQ(0x2222, block.get<2>());

    EXPECT_EQ(OptionalComplex3Str2, to_string(block));
    EXPECT_EQ(OptionalComplex3Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(true, second.has_value());
    EXPECT_EQ("abcdefghijklm", second.value().get<0>());
    EXPECT_EQ("nopqrstuvwxyz", second.value().get<1>());
    EXPECT_EQ(0x2222, third);
#   endif
}

TEST(Optional_TestCase, Strange1)
{
    builder<optional<optional<uint32_t>>> builder;

    EXPECT_EQ(12, sizeof(builder)); // (4 + 1 + padding3) + 2 + padding2

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(1, block.length()); // 1 + 0

    EXPECT_EQ(OptionalStrange1Str, to_string(block));
    EXPECT_EQ(OptionalStrange1Data, block.span());

    builder.set<0>(0x1111);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(5, block.length()); // 1 + 4
    EXPECT_EQ(0x1111, block.get<0>());

    EXPECT_EQ(OptionalStrange1Str2, to_string(block));
    EXPECT_EQ(OptionalStrange1Data2, block.span());
}

TEST(Optional_TestCase, Strange2)
{
    builder<optional<optional<optional<uint32_t>>>> builder;

    EXPECT_EQ(16, sizeof(builder)); // ((4 + 1 + padding3) + 1 + padding3) + 1 + padding3

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(1, block.length()); // 1 + 0

    EXPECT_EQ(OptionalStrange2Str, to_string(block));
    EXPECT_EQ(OptionalStrange2Data, block.span());

    builder.set<0>(0x1111);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(5, block.length()); // 1 + 4
    EXPECT_EQ(0x1111, block.get<0>());

    EXPECT_EQ(OptionalStrange2Str2, to_string(block));
    EXPECT_EQ(OptionalStrange2Data2, block.span());
}

TEST(Optional_TestCase, Strange3)
{
    builder<optional<optional<string>>> builder;

    EXPECT_EQ(48, sizeof(builder)); // (32 + 1 + padding7) + 2 + padding6

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(1, block.length()); // 1 + 0

    EXPECT_EQ(OptionalStrange3Str, to_string(block));
    EXPECT_EQ(OptionalStrange3Data, block.span());

    builder.set<0>("abcdefghijklmnopqrstuvwxyz");

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(28, block.length()); // 1 + (26 + 1)
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.get<0>());

    EXPECT_EQ(OptionalStrange3Str2, to_string(block));
    EXPECT_EQ(OptionalStrange3Data2, block.span());
}

TEST(Optional_TestCase, Simple1Wrapped)
{
    OptionalSimple1Block::builder builder;

    EXPECT_EQ(16, sizeof(builder)); // 4 + (4 + 1 + padding3) + 4

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(9, block.length()); // 1 + 4 + 0 + 4
    EXPECT_EQ(nullopt, block.second());

    EXPECT_EQ(OptionalSimple1Str, to_string(block));
    EXPECT_EQ(OptionalSimple1Data, block.span());

    builder.first(0x1111);
    builder.second(0x2222);
    builder.third(0x3333);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(13, block.length()); // 1 + 4 + 4 + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_EQ(0x2222, block.second());
    EXPECT_EQ(0x3333, block.third());

    EXPECT_EQ(
        OptionalSimple1Block::builder()
            .first(0x1111)
            .second(0x2222)
            .third(0x3333)
            .build(),
        block);

    EXPECT_EQ(OptionalSimple1Str2, to_string(block));
    EXPECT_EQ(OptionalSimple1Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second);
    EXPECT_EQ(0x3333, third);
#   endif
}

TEST(Optional_TestCase, Simple2Wrapped)
{
    OptionalSimple2Block::builder builder;

    EXPECT_EQ(72, sizeof(builder)); // 32 + (4 + 1 + padding3) + 32

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(5, block.length()); // 3 + (0 + 1) + 0 + (0 + 1)
    EXPECT_STREQ("", block.first());
    EXPECT_EQ(nullopt, block.second());
    EXPECT_STREQ("", block.third());

    EXPECT_EQ(OptionalSimple2Str, to_string(block));
    EXPECT_EQ(OptionalSimple2Data, block.span());

    builder.first("abcdefghijklm");
    builder.second(0x1111);
    builder.third("nopqrstuvwxyz");

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(35, block.length()); // 3 + (13 + 1) + 4 + (13 + 1)
    EXPECT_STREQ("abcdefghijklm", block.first());
    EXPECT_EQ(0x1111, block.second());
    EXPECT_STREQ("nopqrstuvwxyz", block.third());

    EXPECT_EQ(OptionalSimple2Str2, to_string(block));
    EXPECT_EQ(OptionalSimple2Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(0x1111, second);
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(Optional_TestCase, Simple3Wrapped)
{
    OptionalSimple3Block::builder builder;

    EXPECT_EQ(48, sizeof(builder)); // 4 + (32 + 1 + padding7) + 4

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(9, block.length()); // 1 + 4 + 0 + 4
    EXPECT_EQ(0, block.first());
    EXPECT_EQ(nullopt, block.second());
    EXPECT_EQ(0, block.third());

    EXPECT_EQ(OptionalSimple3Str, to_string(block));
    EXPECT_EQ(OptionalSimple3Data, block.span());

    builder.first(0x1111);
    builder.second("abcdefghijklmnopqrstuvwxyz");
    builder.third(0x2222);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(36, block.length()); // 1 + 4 + (26 + 1) + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.second());
    EXPECT_EQ(0x2222, block.third());

    EXPECT_EQ(OptionalSimple3Str2, to_string(block));
    EXPECT_EQ(OptionalSimple3Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", second);
    EXPECT_EQ(0x2222, third);
#   endif
}

TEST(Optional_TestCase, Complex1Wrapped)
{
    OptionalComplex1Block::builder builder;

    EXPECT_EQ(17, sizeof(builder)); // 4 + ((4 + 4) + 1) + 4

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(9, block.length()); // 1 + 4 + 0 + 4
    EXPECT_EQ(false, block.has_second());

    EXPECT_EQ(OptionalComplex1Str, to_string(block));
    EXPECT_EQ(OptionalComplex1Data, block.span());

    builder.first(0x1111);
    builder.second(IntPairBlock().first(0x2222).second(0x3333));
    builder.third(0x4444);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(17, block.length()); // 1 + 4 + (4 + 4) + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_EQ(true, block.has_second());
    EXPECT_EQ(0x2222, block.second().first());
    EXPECT_EQ(0x3333, block.second().second());
    EXPECT_EQ(0x4444, block.third());

    EXPECT_EQ(OptionalComplex1Str2, to_string(block));
    EXPECT_EQ(OptionalComplex1Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(true, second.has_value());
    EXPECT_EQ(0x2222, second.value().first());
    EXPECT_EQ(0x3333, second.value().second());
    EXPECT_EQ(0x4444, third);
#   endif
}

TEST(Optional_TestCase, Complex2Wrapped)
{
    OptionalComplex2Block::builder builder;

    EXPECT_EQ(73, sizeof(builder)); // 32 + ((4 + 4) + 1) + 32

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(5, block.length()); // 3 + (0 + 1) + 0 + (0 + 1)
    EXPECT_STREQ("", block.first());
    EXPECT_EQ(false, block.has_second());
    EXPECT_STREQ("", block.third());

    EXPECT_EQ(OptionalComplex2Str, to_string(block));
    EXPECT_EQ(OptionalComplex2Data, block.span());

    builder.first("abcdefghijklm");
    builder.second(IntPairBlock().first(0x1111).second(0x2222));
    builder.third("nopqrstuvwxyz");

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(39, block.length()); // 3 + (13 + 1) + (4 + 4) + (13 + 1)
    EXPECT_STREQ("abcdefghijklm", block.first());
    EXPECT_EQ(true, block.has_second());
    EXPECT_EQ(0x1111, block.second().first());
    EXPECT_EQ(0x2222, block.second().second());
    EXPECT_STREQ("nopqrstuvwxyz", block.third());

    EXPECT_EQ(OptionalComplex2Str2, to_string(block));
    EXPECT_EQ(OptionalComplex2Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(true, second.has_value());
    EXPECT_EQ(0x1111, second.value().first());
    EXPECT_EQ(0x2222, second.value().second());
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(Optional_TestCase, Complex3Wrapped)
{
    OptionalComplex3Block::builder builder;

    EXPECT_EQ(73, sizeof(builder)); // 4 + ((32 + 32) + 1) + 4

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(9, block.length()); // 1 + 4 + 0 + 4
    EXPECT_EQ(0, block.first());
    EXPECT_EQ(false, block.has_second());
    EXPECT_EQ(0, block.third());

    EXPECT_EQ(OptionalComplex3Str, to_string(block));
    EXPECT_EQ(OptionalComplex3Data, block.span());

    builder.first(0x1111);
    builder.second(StringPairBuilder().first("abcdefghijklm").second("nopqrstuvwxyz"));
    builder.third(0x2222);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(39, block.length()); // 1 + 4 + (2 + (13 + 1) + (13 + 1)) + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_EQ(true, block.has_second());
    EXPECT_STREQ("abcdefghijklm", block.second().first());
    EXPECT_STREQ("nopqrstuvwxyz", block.second().second());
    EXPECT_EQ(0x2222, block.third());

    EXPECT_EQ(OptionalComplex3Str2, to_string(block));
    EXPECT_EQ(OptionalComplex3Data2, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(true, second.has_value());
    EXPECT_STREQ("abcdefghijklm", second.value().first());
    EXPECT_STREQ("nopqrstuvwxyz", second.value().second());
    EXPECT_EQ(0x2222, third);
#   endif
}
