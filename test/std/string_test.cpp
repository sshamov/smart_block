#include <gtest/gtest.h>

#include "../test_utils.h"
#include "string_test_wrappers.h"
#include "string_test_data.h"

using namespace std;
using namespace smart_block;

TEST(ConstructBare_TestCase, StringSimple1)
{
    builder<string, uint32_t, string> builder;

    EXPECT_EQ(68, sizeof(builder)); // 32 + 4 + 32

    builder.set<0>("abcdefghijklm");
    builder.set<1>(0x1111);
    builder.set<2>("nopqrstuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(34, block.length()); // 2 + (13 + 1) + 4 + (13 + 1)
    EXPECT_EQ("abcdefghijklm", block.get<0>());
    EXPECT_EQ(0x1111, block.get<1>());
    EXPECT_EQ("nopqrstuvwxyz", block.get<2>());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(0x1111, second);
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(ConstructBare_TestCase, StringSimple2)
{
    builder<uint32_t, string, uint32_t> builder;

    EXPECT_EQ(40, sizeof(builder)); // 4 + 32 + 4

    builder.set<0>(0x1111);
    builder.set<1>("abcdefghijklmnopqrstuvwxyz");
    builder.set<2>(0x2222);

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(36, block.length()); // 1 + 4 + (26 + 1) + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.get<1>());
    EXPECT_EQ(0x2222, block.get<2>());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", second);
    EXPECT_EQ(0x2222, third);
#   endif
}

TEST(ConstructBare_TestCase, StringComplex1)
{
    builder<string, block<uint32_t, uint32_t>, string> builder;

    EXPECT_EQ(72, sizeof(builder)); // 32 + (4 + 4) + 32

    builder.set<0>("abcdefghijklm");
    builder.sub<1>().set<0>(0x1111);
    builder.sub<1>().set<1>(0x2222);
    builder.set<2>("nopqrstuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(38, block.length()); // 2 + (13 + 1) + (4 + 4) + (13 + 1)
    EXPECT_EQ("abcdefghijklm", block.get<0>());
    EXPECT_EQ(0x1111, block.sub<1>().get<0>());
    EXPECT_EQ(0x2222, block.sub<1>().get<1>());
    EXPECT_EQ("nopqrstuvwxyz", block.get<2>());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(0x1111, second.get<0>());
    EXPECT_EQ(0x2222, second.get<1>());
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(ConstructBare_TestCase, StringComplex2)
{
    builder<uint32_t, block<string, string>, uint32_t> builder;

    EXPECT_EQ(72, sizeof(builder)); // 4 + (32 + 32) + 4

    builder.set<0>(0x1111);
    builder.sub<1>().set<0>("abcdefghijklm");
    builder.sub<1>().set<1>("nopqrstuvwxyz");
    builder.set<2>(0x2222);

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(39, block.length()); // 1 + 4 + (2 + (13 + 1) + (13 + 1)) + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ("abcdefghijklm", block.sub<1>().get<0>());
    EXPECT_EQ("nopqrstuvwxyz", block.sub<1>().get<1>());
    EXPECT_EQ(0x2222, block.get<2>());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ("abcdefghijklm", second.get<0>());
    EXPECT_EQ("nopqrstuvwxyz", second.get<1>());
    EXPECT_EQ(0x2222, third);
#   endif
}

TEST(ConstructBare_TestCase, StringVeryComplex1)
{
    builder<string, block<uint32_t, block<string, string>, uint32_t>, string> builder;

    EXPECT_EQ(136, sizeof(builder)); // 32 + (4 + (32 + 32) + 4) + 32

    builder.set<0>("abcdefg");
    builder.sub<1>().set<0>(0x1111);
    builder.sub<1>().sub<1>().set<0>("hijklm");
    builder.sub<1>().sub<1>().set<1>("nopqrs");
    builder.sub<1>().set<2>(0x2222);
    builder.set<2>("tuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(44, block.length()); // 3 + (7 + 1) + (1 + 4 + (2 + (6 + 1) + (6 + 1)) + 4) + (7 + 1)
    EXPECT_EQ("abcdefg", block.get<0>());
    EXPECT_EQ(0x1111, block.sub<1>().get<0>());
    EXPECT_EQ("hijklm", block.sub<1>().sub<1>().get<0>());
    EXPECT_EQ("nopqrs", block.sub<1>().sub<1>().get<1>());
    EXPECT_EQ(0x2222, block.sub<1>().get<2>());
    EXPECT_EQ("tuvwxyz", block.get<2>());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefg", first);
    EXPECT_EQ(0x1111, second.get<0>());
    EXPECT_EQ("hijklm", second.sub<1>().get<0>());
    EXPECT_EQ("nopqrs", second.sub<1>().get<1>());
    EXPECT_EQ(0x2222, second.get<2>());
    EXPECT_EQ("tuvwxyz", third);
#   endif
}

TEST(ConstructBare_TestCase, StringVeryComplex2)
{
    builder<uint32_t, block<string, block<uint32_t, uint32_t>, string>, uint32_t> builder;

    EXPECT_EQ(80, sizeof(builder)); // 4 + (32 + (4 + 4) + 32) + 4

    builder.set<0>(0x1111);
    builder.sub<1>().set<0>("abcdefghijklm");
    builder.sub<1>().sub<1>().set<0>(0x2222);
    builder.sub<1>().sub<1>().set<1>(0x3333);
    builder.sub<1>().set<2>("nopqrstuvwxyz");
    builder.set<2>(0x4444);

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(47, block.length()); // 1 + 4 + (2 + (13 + 1) + (4 + 4) + (13 + 1)) + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ("abcdefghijklm", block.sub<1>().get<0>());
    EXPECT_EQ(0x2222, block.sub<1>().sub<1>().get<0>());
    EXPECT_EQ(0x3333, block.sub<1>().sub<1>().get<1>());
    EXPECT_EQ("nopqrstuvwxyz", block.sub<1>().get<2>());
    EXPECT_EQ(0x4444, block.get<2>());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ("abcdefghijklm", second.get<0>());
    EXPECT_EQ(0x2222, second.sub<1>().get<0>());
    EXPECT_EQ(0x3333, second.sub<1>().get<1>());
    EXPECT_EQ("nopqrstuvwxyz", second.get<2>());
    EXPECT_EQ(0x4444, third);
#   endif
}

TEST(ConstructWrapped_TestCase, StringSimple1)
{
    StringSimple1Block::builder builder;

    EXPECT_EQ(68, sizeof(builder)); // 32 + 4 + 32

    builder.first("abcdefghijklm");
    builder.second(0x1111);
    builder.third("nopqrstuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(34, block.length()); // 2 + (13 + 1) + 4 + (13 + 1)
    EXPECT_STREQ("abcdefghijklm", block.first());
    EXPECT_EQ(0x1111, block.second());
    EXPECT_STREQ("nopqrstuvwxyz", block.third());

    EXPECT_EQ(StringSimple1Str, to_string(block));
    EXPECT_EQ(StringSimple1Data, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(0x1111, second);
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(ConstructWrapped_TestCase, StringSimple2)
{
    StringSimple2Block::builder builder;

    EXPECT_EQ(40, sizeof(builder)); // 4 + 32 + 4

    builder.first(0x1111);
    builder.second("abcdefghijklmnopqrstuvwxyz");
    builder.third(0x2222);

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(36, block.length()); // 1 + 4 + (26 + 1) + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_STREQ("abcdefghijklmnopqrstuvwxyz", block.second());
    EXPECT_EQ(0x2222, block.third());

    EXPECT_EQ(StringSimple2Str, to_string(block));
    EXPECT_EQ(StringSimple2Data, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", second);
    EXPECT_EQ(0x2222, third);
#   endif
}

TEST(ConstructWrapped_TestCase, StringComplex1)
{
    StringComplex1Block::builder builder;

    EXPECT_EQ(72, sizeof(builder)); // 32 + (4 + 4) + 32

    builder.first("abcdefghijklm");
    builder.second(IntPairBlock().first(0x1111).second(0x2222));
    builder.third("nopqrstuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(38, block.length()); // 2 + (13 + 1) + (4 + 4) + (13 + 1)
    EXPECT_STREQ("abcdefghijklm", block.first());
    EXPECT_EQ(0x1111, block.second().first());
    EXPECT_EQ(0x2222, block.second().second());
    EXPECT_STREQ("nopqrstuvwxyz", block.third());

    EXPECT_EQ(
        StringComplex1Block::builder()
            .first("abcdefghijklm")
            .second(IntPairBlock().first(0x1111).second(0x2222))
            .third("nopqrstuvwxyz")
            .build(),
        block);
    EXPECT_EQ(IntPairBlock().first(0x1111).second(0x2222), block.second());

    EXPECT_EQ(StringComplex1Str, to_string(block));
    EXPECT_EQ(StringComplex1Str2, to_string2(block));
    EXPECT_EQ(StringComplex1Data, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(0x1111, second.first());
    EXPECT_EQ(0x2222, second.second());
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(ConstructWrapped_TestCase, StringComplex2)
{
    StringComplex2Block::builder builder;

    EXPECT_EQ(72, sizeof(builder)); // 4 + 32 + 32 + 4

    builder.first(0x1111);
    builder.second(StringPairBuilder().first("abcdefghijklm").second("nopqrstuvwxyz"));
    builder.third(0x2222);

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(39, block.length()); // 1 + 4 + (2 + (13 + 1) + (13 + 1)) + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_STREQ("abcdefghijklm", block.second().first());
    EXPECT_STREQ("nopqrstuvwxyz", block.second().second());
    EXPECT_EQ(0x2222, block.third());

    EXPECT_EQ(
        StringComplex2Block::builder()
            .first(0x1111)
            .second(StringPairBuilder().first("abcdefghijklm").second("nopqrstuvwxyz"))
            .third(0x2222)
            .build(),
        block);
    EXPECT_EQ(
        StringPairBuilder()
            .first("abcdefghijklm")
            .second("nopqrstuvwxyz")
            .build(),
        block.second().as_base());

    EXPECT_EQ(StringComplex2Str, to_string(block));
    EXPECT_EQ(StringComplex2Str2, to_string2(block));
    EXPECT_EQ(StringComplex2Data, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_STREQ("abcdefghijklm", second.first());
    EXPECT_STREQ("nopqrstuvwxyz", second.second());
    EXPECT_EQ(0x2222, third);
#   endif
}

TEST(ConstructWrapped_TestCase, StringVeryComplex1)
{
    StringVeryComplex1Block::builder builder;

    EXPECT_EQ(136, sizeof(builder)); // 32 + (4 + (32 + 32) + 4) + 32

    builder.first("abcdefg");
    builder.second(StringComplex2Builder()
        .first(0x1111)
        .second(StringPairBuilder().first("hijklm").second("nopqrs"))
        .third(0x2222));
    builder.third("tuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(44, block.length()); // 3 + (7 + 1) + (1 + 4 + (2 + (6 + 1) + (6 + 1)) + 4) + (7 + 1)
    EXPECT_STREQ("abcdefg", block.first());
    EXPECT_EQ(0x1111, block.second().first());
    EXPECT_STREQ("hijklm", block.second().second().first());
    EXPECT_STREQ("nopqrs", block.second().second().second());
    EXPECT_EQ(0x2222, block.second().third());
    EXPECT_STREQ("tuvwxyz", block.third());

    EXPECT_EQ(
        StringVeryComplex1Block::builder()
            .first("abcdefg")
            .second(StringComplex2Builder()
                .first(0x1111)
                .second(StringPairBuilder().first("hijklm").second("nopqrs"))
                .third(0x2222))
            .third("tuvwxyz")
            .build(),
        block);
    EXPECT_EQ(
        StringComplex2Builder()
            .first(0x1111)
            .second(StringPairBuilder().first("hijklm").second("nopqrs"))
            .third(0x2222)
            .build(),
        block.second());
    EXPECT_EQ(
        StringPairBuilder()
            .first("hijklm")
            .second("nopqrs")
            .build(),
        block.second().second().as_base());

    EXPECT_EQ(StringVeryComplex1Str, to_string(block));
    EXPECT_EQ(StringVeryComplex1Str2, to_string2(block));
    EXPECT_EQ(StringVeryComplex1Data, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefg", first);
    EXPECT_EQ(0x1111, second.first());
    EXPECT_STREQ("hijklm", second.second().first());
    EXPECT_STREQ("nopqrs", second.second().second());
    EXPECT_EQ(0x2222, second.third());
    EXPECT_EQ("tuvwxyz", third);
#   endif
}

TEST(ConstructWrapped_TestCase, StringVeryComplex2)
{
    StringVeryComplex2Block::builder builder;

    EXPECT_EQ(80, sizeof(builder)); // 4 + (32 + (4 + 4) + 32) + 4

    builder.first(0x1111);
    builder.second(StringComplex1Builder()
        .first("abcdefghijklm")
        .second(IntPairBlock().first(0x2222).second(0x3333))
        .third("nopqrstuvwxyz"));
    builder.third(0x4444);

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(47, block.length()); // 1 + 4 + (2 + (13 + 1) + (4 + 4) + (13 + 1)) + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_STREQ("abcdefghijklm", block.second().first());
    EXPECT_EQ(0x2222, block.second().second().first());
    EXPECT_EQ(0x3333, block.second().second().second());
    EXPECT_STREQ("nopqrstuvwxyz", block.second().third());
    EXPECT_EQ(0x4444, block.third());

    EXPECT_EQ(
        StringVeryComplex2Block::builder()
            .first(0x1111)
            .second(StringComplex1Builder()
                .first("abcdefghijklm")
                .second(IntPairBlock().first(0x2222).second(0x3333))
                .third("nopqrstuvwxyz"))
            .third(0x4444)
            .build(),
        block);
    EXPECT_EQ(
        StringComplex1Builder()
            .first("abcdefghijklm")
            .second(IntPairBlock().first(0x2222).second(0x3333))
            .third("nopqrstuvwxyz")
            .build(),
        block.second());
    EXPECT_EQ(IntPairBlock().first(0x2222).second(0x3333), block.second().second());

    EXPECT_EQ(StringVeryComplex2Str, to_string(block));
    EXPECT_EQ(StringVeryComplex2Str2, to_string2(block));
    EXPECT_EQ(StringVeryComplex2Data, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_STREQ("abcdefghijklm", second.first());
    EXPECT_EQ(0x2222, second.second().first());
    EXPECT_EQ(0x3333, second.second().second());
    EXPECT_STREQ("nopqrstuvwxyz", second.third());
    EXPECT_EQ(0x4444, third);
#   endif
}

TEST(Dissect_TestCase, StringSimple1)
{
    StringSimple1CFacade facade(StringSimple1Data.data());

    EXPECT_EQ(8, sizeof(facade));
    EXPECT_EQ(StringSimple1Data.size(), facade.length());
    EXPECT_STREQ("abcdefghijklm", facade.first());
    EXPECT_EQ(0x1111, facade.second());
    EXPECT_STREQ("nopqrstuvwxyz", facade.third());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = facade;

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(0x1111, second);
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(Dissect_TestCase, StringSimple2)
{
    StringSimple2CFacade facade(StringSimple2Data.data());

    EXPECT_EQ(8, sizeof(facade));
    EXPECT_EQ(StringSimple2Data.size(), facade.length());
    EXPECT_EQ(0x1111, facade.first());
    EXPECT_STREQ("abcdefghijklmnopqrstuvwxyz", facade.second());
    EXPECT_EQ(0x2222, facade.third());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = facade;

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", second);
    EXPECT_EQ(0x2222, third);
#   endif
}

TEST(Dissect_TestCase, StringComplex1)
{
    StringComplex1CFacade facade(StringComplex1Data.data());

    EXPECT_EQ(8, sizeof(facade));
    EXPECT_EQ(StringComplex1Data.size(), facade.length());
    EXPECT_STREQ("abcdefghijklm", facade.first());
    EXPECT_EQ(0x1111, facade.second().first());
    EXPECT_EQ(0x2222, facade.second().second());
    EXPECT_STREQ("nopqrstuvwxyz", facade.third());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = facade;

    EXPECT_EQ("abcdefghijklm", first);
    EXPECT_EQ(0x1111, second.first());
    EXPECT_EQ(0x2222, second.second());
    EXPECT_EQ("nopqrstuvwxyz", third);
#   endif
}

TEST(Dissect_TestCase, StringComplex2)
{
    StringComplex2CFacade facade(StringComplex2Data.data());

    EXPECT_EQ(8, sizeof(facade));
    EXPECT_EQ(StringComplex2Data.size(), facade.length());
    EXPECT_EQ(0x1111, facade.first());
    EXPECT_STREQ("abcdefghijklm", facade.second().first());
    EXPECT_STREQ("nopqrstuvwxyz", facade.second().second());
    EXPECT_EQ(0x2222, facade.third());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = facade;

    EXPECT_EQ(0x1111, first);
    EXPECT_STREQ("abcdefghijklm", second.first());
    EXPECT_STREQ("nopqrstuvwxyz", second.second());
    EXPECT_EQ(0x2222, third);
#   endif
}

TEST(Dissect_TestCase, StringVeryComplex1)
{
    StringVeryComplex1CFacade facade(StringVeryComplex1Data.data());

    EXPECT_EQ(8, sizeof(facade));
    EXPECT_EQ(StringVeryComplex1Data.size(), facade.length());
    EXPECT_STREQ("abcdefg", facade.first());
    EXPECT_EQ(0x1111, facade.second().first());
    EXPECT_STREQ("hijklm", facade.second().second().first());
    EXPECT_STREQ("nopqrs", facade.second().second().second());
    EXPECT_EQ(0x2222, facade.second().third());
    EXPECT_STREQ("tuvwxyz", facade.third());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = facade;

    EXPECT_EQ("abcdefg", first);
    EXPECT_EQ(0x1111, second.first());
    EXPECT_STREQ("hijklm", second.second().first());
    EXPECT_STREQ("nopqrs", second.second().second());
    EXPECT_EQ(0x2222, second.third());
    EXPECT_EQ("tuvwxyz", third);
#   endif
}

TEST(Dissect_TestCase, StringVeryComplex2)
{
    StringVeryComplex2CFacade facade(StringVeryComplex2Data.data());

    EXPECT_EQ(8, sizeof(facade));
    EXPECT_EQ(StringVeryComplex2Data.size(), facade.length());
    EXPECT_EQ(0x1111, facade.first());
    EXPECT_STREQ("abcdefghijklm", facade.second().first());
    EXPECT_EQ(0x2222, facade.second().second().first());
    EXPECT_EQ(0x3333, facade.second().second().second());
    EXPECT_STREQ("nopqrstuvwxyz", facade.second().third());
    EXPECT_EQ(0x4444, facade.third());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = facade;

    EXPECT_EQ(0x1111, first);
    EXPECT_STREQ("abcdefghijklm", second.first());
    EXPECT_EQ(0x2222, second.second().first());
    EXPECT_EQ(0x3333, second.second().second());
    EXPECT_STREQ("nopqrstuvwxyz", second.third());
    EXPECT_EQ(0x4444, third);
#   endif
}

TEST(Assignment_TestCase, StringAssignBare)
{
    builder<string> builder;

    builder.set<0>("abcdefghijklm");

    auto src = builder.build();

    builder.set<0>("");

    auto dst = builder.build();

    EXPECT_EQ("", dst.get<0>());

    dst = src;

    memset(src.data(), 0, src.length());

    EXPECT_EQ("abcdefghijklm", dst.get<0>());
}

TEST(Assignment_TestCase, StringMoveBare)
{
    builder<string> builder;

    builder.set<0>("abcdefghijklm");

    auto src = builder.build();

    builder.set<0>("");

    auto dst = builder.build();

    EXPECT_EQ("", dst.get<0>());

    dst = move(src);

    EXPECT_EQ(nullptr, src.data());
    EXPECT_EQ("abcdefghijklm", dst.get<0>());
}

TEST(Assignment_TestCase, StringAssignWrapped)
{
    StringSimple1Block::builder builder;

    builder.first("abcdefghijklm");
    builder.second(0x1111);
    builder.third("nopqrstuvwxyz");

    auto src = builder.build();

    builder.first("");
    builder.second(0);
    builder.third("");

    auto dst = builder.build();

    EXPECT_STREQ("", dst.first());
    EXPECT_EQ(0, dst.second());
    EXPECT_STREQ("", dst.third());

    dst = src;

    memset(src.data(), 0, src.length());

    EXPECT_STREQ("abcdefghijklm", dst.first());
    EXPECT_EQ(0x1111, dst.second());
    EXPECT_STREQ("nopqrstuvwxyz", dst.third());
}

TEST(Assignment_TestCase, StringMoveWrapped)
{
    StringSimple1Block::builder builder;

    builder.first("abcdefghijklm");
    builder.second(0x1111);
    builder.third("nopqrstuvwxyz");

    auto src = builder.build();

    builder.first("");
    builder.second(0);
    builder.third("");

    auto dst = builder.build();

    EXPECT_STREQ("", dst.first());
    EXPECT_EQ(0, dst.second());
    EXPECT_STREQ("", dst.third());

    dst = move(src);

    EXPECT_EQ(nullptr, src.data());
    EXPECT_STREQ("abcdefghijklm", dst.first());
    EXPECT_EQ(0x1111, dst.second());
    EXPECT_STREQ("nopqrstuvwxyz", dst.third());
}

struct string_struct { std::string value; };

template <>
struct transform_item<string_struct>
{
    static constexpr bool is_plain = false;

    using value_type = string_struct;
    using builder    = string_struct;
    using editor     = string_struct&;

    static editor get_editor(builder& builder) { return builder; }

    static size_t length(const builder& builder) { return builder.value.size() + 1; }

    static void pack(const builder& builder, byte* target)
    { strcpy(reinterpret_cast<char*>(target), builder.value.c_str()); }

    static value_type unpack(const byte* target, size_t length)
    { return { string(reinterpret_cast<const char*>(target), length - 1) }; }
};

TEST(Misc_TestCase, StringAggregatable)
{
    builder<string_struct, string_struct> builder;

    EXPECT_EQ(64, sizeof(builder)); // 32 + 32

    builder.set<0>("abcdefghijklm");
    builder.set<1>("nopqrstuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(30, block.length()); // 2 + (13 + 1) + (13 + 1)
    EXPECT_EQ("abcdefghijklm", block.get<0>().value);
    EXPECT_EQ("nopqrstuvwxyz", block.get<1>().value);

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ("abcdefghijklm", first.value);
    EXPECT_EQ("nopqrstuvwxyz", second.value);
#   endif
}
