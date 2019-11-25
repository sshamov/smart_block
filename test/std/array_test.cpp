#include <gtest/gtest.h>

#include "../../block.h"
#include "../../builder.h"
#include "../../std/array.h"
#include "../../std/string.h"

#include "../test_utils.h"
#include "array_test_data.h"

using namespace std;
using namespace smart_block;

struct int_struct { int value; };

ostream& operator <<(ostream& os, const int_struct& val);

TEST(Array_TestCase, Simple1)
{
    block<array<uint32_t, 3>> block;

    block.set<0>({ 0x1111, 0x2222, 0x3333 });

    EXPECT_EQ(24, sizeof(block));  // length() + padding4 + 8
    EXPECT_EQ(12, block.length()); // 4 * 3
    EXPECT_EQ(0x1111, block.get<0>()[0]);
    EXPECT_EQ(0x2222, block.get<0>()[1]);
    EXPECT_EQ(0x3333, block.get<0>()[2]);

    EXPECT_EQ(ArraySimple1Str, to_string(block));
    EXPECT_EQ(ArraySimple1Data, block.span());
}

TEST(Array_TestCase, Simple2)
{
    block<array<int_struct, 3>> block;

    block.edit<0>()[0] = { 0x1111 };
    block.edit<0>()[1] = { 0x2222 };
    block.edit<0>()[2] = { 0x3333 };

    EXPECT_EQ(24, sizeof(block));  // length() + padding4 + 8
    EXPECT_EQ(12, block.length()); // 4 * 3
    EXPECT_EQ(0x1111, block.get<0>()[0].value);
    EXPECT_EQ(0x2222, block.get<0>()[1].value);
    EXPECT_EQ(0x3333, block.get<0>()[2].value);

    EXPECT_EQ(ArraySimple2Str, to_string(block));
    EXPECT_EQ(ArraySimple2Data, block.span());
}

TEST(Array_TestCase, Simple3)
{
    builder<array<string, 3>> builder;

    EXPECT_EQ(96, sizeof(builder)); // 32 * 3

    builder.edit<0>()[0] = "abcdefghi";
    builder.edit<0>()[1] = "jklmnopqr";
    builder.edit<0>()[2] = "stuvwxyz";

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(33, block.length()); // 1 + (3 + (9 + 1) + (9 + 1) + (8 + 1))
    EXPECT_EQ("abcdefghi", block.get<0>()[0]);
    EXPECT_EQ("jklmnopqr", block.get<0>()[1]);
    EXPECT_EQ("stuvwxyz",  block.get<0>()[2]);
    EXPECT_STREQ("abcdefghi", reinterpret_cast<const char*>(block.get<0>().raw_data(0).data()));
    EXPECT_STREQ("jklmnopqr", reinterpret_cast<const char*>(block.get<0>().raw_data(1).data()));
    EXPECT_STREQ("stuvwxyz",  reinterpret_cast<const char*>(block.get<0>().raw_data(2).data()));

    EXPECT_EQ(ArraySimple3Str, to_string(block));
    EXPECT_EQ(ArraySimple3Data, block.span());
}

TEST(Array_TestCase, Block1)
{
    block<array<block<uint32_t, uint32_t>, 3>> block;

    block.edit<0>()[0].set<0>(0x1111);
    block.edit<0>()[0].set<1>(0x2222);
    block.edit<0>()[1].set<0>(0x3333);
    block.edit<0>()[1].set<1>(0x4444);
    block.edit<0>()[2].set<0>(0x5555);
    block.edit<0>()[2].set<1>(0x6666);

    EXPECT_EQ(32, sizeof(block));  // length() + 8
    EXPECT_EQ(24, block.length()); // (4 + 4) * 3
    EXPECT_EQ(0x1111, block.get<0>()[0].get<0>());
    EXPECT_EQ(0x2222, block.get<0>()[0].get<1>());
    EXPECT_EQ(0x3333, block.get<0>()[1].get<0>());
    EXPECT_EQ(0x4444, block.get<0>()[1].get<1>());
    EXPECT_EQ(0x5555, block.get<0>()[2].get<0>());
    EXPECT_EQ(0x6666, block.get<0>()[2].get<1>());

    EXPECT_EQ(ArrayBlock1Str, to_string(block));
    EXPECT_EQ(ArrayBlock1Data, block.span());
}

TEST(Array_TestCase, Block2)
{
    builder<array<block<string, string>, 3>> builder;

    EXPECT_EQ(192, sizeof(builder)); // (32 + 32) * 3

    builder.edit<0>()[0].set<0>("abcde");
    builder.edit<0>()[0].set<1>("fghij");
    builder.edit<0>()[1].set<0>("klmn");
    builder.edit<0>()[1].set<1>("opqr");
    builder.edit<0>()[2].set<0>("stuv");
    builder.edit<0>()[2].set<1>("wxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(42, block.length()); // 1 + (3 + (2 + (5 + 1) + (5 + 1))
                                   //        + (2 + (4 + 1) + (4 + 1))
                                   //        + (2 + (4 + 1) + (4 + 1)))
    EXPECT_EQ("abcde", block.get<0>()[0].get<0>());
    EXPECT_EQ("fghij", block.get<0>()[0].get<1>());
    EXPECT_EQ("klmn",  block.get<0>()[1].get<0>());
    EXPECT_EQ("opqr",  block.get<0>()[1].get<1>());
    EXPECT_EQ("stuv",  block.get<0>()[2].get<0>());
    EXPECT_EQ("wxyz",  block.get<0>()[2].get<1>());

    EXPECT_EQ(ArrayBlock2Str, to_string(block));
    EXPECT_EQ(ArrayBlock2Data, block.span());
}

TEST(Array_TestCase, Block3)
{
    builder<array<block<uint32_t, string>, 3>> builder;

    EXPECT_EQ(108, sizeof(builder)); // (32 + 4) * 3

    builder.edit<0>()[0].set<0>(0x1111);
    builder.edit<0>()[0].set<1>("abcdefghi");
    builder.edit<0>()[1].set<0>(0x2222);
    builder.edit<0>()[1].set<1>("jklmnopqr");
    builder.edit<0>()[2].set<0>(0x3333);
    builder.edit<0>()[2].set<1>("stuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(48, block.length()); // 1 + (3 + (1 + 4 + (9 + 1))
                                   //        + (1 + 4 + (9 + 1))
                                   //        + (1 + 4 + (8 + 1)))
    EXPECT_EQ(0x1111,      block.get<0>()[0].get<0>());
    EXPECT_EQ("abcdefghi", block.get<0>()[0].get<1>());
    EXPECT_EQ(0x2222,      block.get<0>()[1].get<0>());
    EXPECT_EQ("jklmnopqr", block.get<0>()[1].get<1>());
    EXPECT_EQ(0x3333,      block.get<0>()[2].get<0>());
    EXPECT_EQ("stuvwxyz",  block.get<0>()[2].get<1>());

    EXPECT_EQ(ArrayBlock3Str, to_string(block));
    EXPECT_EQ(ArrayBlock3Data, block.span());
}

TEST(Array_TestCase, Block4)
{
    builder<array<block<string, uint32_t>, 3>> builder;

    EXPECT_EQ(108, sizeof(builder)); // (4 + 32) * 3

    builder.edit<0>()[0].set<0>("abcdefghi");
    builder.edit<0>()[0].set<1>(0x1111);
    builder.edit<0>()[1].set<0>("jklmnopqr");
    builder.edit<0>()[1].set<1>(0x2222);
    builder.edit<0>()[2].set<0>("stuvwxyz");
    builder.edit<0>()[2].set<1>(0x3333);

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(48, block.length()); // 1 + (3 + (1 + (9 + 1) + 4)
                                   //        + (1 + (9 + 1) + 4)
                                   //        + (1 + (8 + 1) + 4))
    EXPECT_EQ("abcdefghi", block.get<0>()[0].get<0>());
    EXPECT_EQ(0x1111,      block.get<0>()[0].get<1>());
    EXPECT_EQ("jklmnopqr", block.get<0>()[1].get<0>());
    EXPECT_EQ(0x2222,      block.get<0>()[1].get<1>());
    EXPECT_EQ("stuvwxyz",  block.get<0>()[2].get<0>());
    EXPECT_EQ(0x3333,      block.get<0>()[2].get<1>());

    EXPECT_EQ(ArrayBlock4Str, to_string(block));
    EXPECT_EQ(ArrayBlock4Data, block.span());
}
