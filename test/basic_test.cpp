#include <gtest/gtest.h>

#include "basic_test_wrappers.h"
#include "basic_test_data.h"
#include "test_utils.h"

using namespace smart_block;

TEST(ConstructBare_TestCase, Simple)
{
    block<uint32_t, uint32_t> block;

    block.set<0>(0x1111);
    block.set<1>(0x2222);

    EXPECT_EQ(16, sizeof(block)); // length() + 8
    EXPECT_EQ(8, block.length()); // 4 + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ(0x2222, block.get<1>());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second);
#   endif
}

TEST(ConstructBare_TestCase, Complex)
{
    block<uint32_t, block<uint32_t, uint32_t>, uint32_t> block;

    block.set<0>(0x1111);
    block.sub<1>().set<0>(0x2222);
    block.sub<1>().set<1>(0x3333);
    block.set<2>(0x4444);

    EXPECT_EQ(24, sizeof(block));  // length() + 8
    EXPECT_EQ(16, block.length()); // 4 + (4 + 4) + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ(0x2222, block.sub<1>().get<0>());
    EXPECT_EQ(0x3333, block.sub<1>().get<1>());
    EXPECT_EQ(0x4444, block.get<2>());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second.get<0>());
    EXPECT_EQ(0x3333, second.get<1>());
    EXPECT_EQ(0x4444, third);
#   endif
}

TEST(ConstructBare_TestCase, VeryComplex)
{
    block<uint32_t, block<uint32_t, block<uint32_t, uint32_t>, uint32_t>, uint32_t> block;

    block.set<0>(0x1111);
    block.sub<1>().set<0>(0x2222);
    block.sub<1>().sub<1>().set<0>(0x3333);
    block.sub<1>().sub<1>().set<1>(0x4444);
    block.sub<1>().set<2>(0x5555);
    block.set<2>(0x6666);

    EXPECT_EQ(32, sizeof(block));  // 8 + length()
    EXPECT_EQ(24, block.length()); // 4 + (4 + (4 + 4) + 4) + 4
    EXPECT_EQ(0x1111, block.get<0>());
    EXPECT_EQ(0x2222, block.sub<1>().get<0>());
    EXPECT_EQ(0x3333, block.sub<1>().sub<1>().get<0>());
    EXPECT_EQ(0x4444, block.sub<1>().sub<1>().get<1>());
    EXPECT_EQ(0x5555, block.sub<1>().get<2>());
    EXPECT_EQ(0x6666, block.get<2>());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second.get<0>());
    EXPECT_EQ(0x3333, second.sub<1>().get<0>());
    EXPECT_EQ(0x4444, second.sub<1>().get<1>());
    EXPECT_EQ(0x5555, second.get<2>());
    EXPECT_EQ(0x6666, third);
#   endif
}

TEST(ConstructWrapped_TestCase, Simple)
{
    IntPairBlock block;

    block.first(0x1111);
    block.second(0x2222);

    EXPECT_EQ(16, sizeof(block)); // length() + 8
    EXPECT_EQ(8, block.length()); // 4 + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_EQ(0x2222, block.second());

    EXPECT_EQ(IntPairBlock().first(0x1111).second(0x2222), block);

    EXPECT_EQ(SimpleStr, to_string(block));
    EXPECT_EQ(SimpleData, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second);
#   endif
}

TEST(ConstructWrapped_TestCase, Complex)
{
    ComplexBlock block;

    block.first(0x1111);
    block.second(IntPairBlock().first(0x2222).second(0x3333));
    block.third(0x4444);

    EXPECT_EQ(24, sizeof(block));  // length() + 8
    EXPECT_EQ(16, block.length()); // 4 + (4 + 4) + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_EQ(0x2222, block.second().first());
    EXPECT_EQ(0x3333, block.second().second());
    EXPECT_EQ(0x4444, block.third());

    EXPECT_EQ(
        ComplexBlock()
            .first(0x1111)
            .second(IntPairBlock().first(0x2222).second(0x3333))
            .third(0x4444),
        block);
    EXPECT_EQ(IntPairBlock().first(0x2222).second(0x3333), block.second());

    EXPECT_EQ(ComplexStr, to_string(block));
    EXPECT_EQ(ComplexStr2, to_string2(block));
    EXPECT_EQ(ComplexData, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second.first());
    EXPECT_EQ(0x3333, second.second());
    EXPECT_EQ(0x4444, third);
#   endif
}

TEST(ConstructWrapped_TestCase, VeryComplex)
{
    VeryComplexBlock block;

    block.first(0x1111);
    block.second(ComplexBlock()
        .first(0x2222)
        .second(IntPairBlock().first(0x3333).second(0x4444))
        .third(0x5555));
    block.third(0x6666);

    EXPECT_EQ(32, sizeof(block));  // length() + 8
    EXPECT_EQ(24, block.length()); // 4 + (4 + (4 + 4) + 4) + 4
    EXPECT_EQ(0x1111, block.first());
    EXPECT_EQ(0x2222, block.second().first());
    EXPECT_EQ(0x3333, block.second().second().first());
    EXPECT_EQ(0x4444, block.second().second().second());
    EXPECT_EQ(0x5555, block.second().third());
    EXPECT_EQ(0x6666, block.third());

    EXPECT_EQ(
        VeryComplexBlock()
            .first(0x1111)
            .second(ComplexBlock()
                .first(0x2222)
                .second(IntPairBlock().first(0x3333).second(0x4444))
                .third(0x5555))
            .third(0x6666),
        block);
    EXPECT_EQ(
        ComplexBlock()
            .first(0x2222)
            .second(IntPairBlock().first(0x3333).second(0x4444))
            .third(0x5555),
        block.second());
    EXPECT_EQ(IntPairBlock().first(0x3333).second(0x4444), block.second().second());

    EXPECT_EQ(VeryComplexStr, to_string(block));
    EXPECT_EQ(VeryComplexStr2, to_string2(block));
    EXPECT_EQ(VeryComplexData, block.span());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second.first());
    EXPECT_EQ(0x3333, second.second().first());
    EXPECT_EQ(0x4444, second.second().second());
    EXPECT_EQ(0x5555, second.third());
    EXPECT_EQ(0x6666, third);
#   endif
}

TEST(Dissect_TestCase, Simple)
{
    IntPairCFacade facade(SimpleData.data());

    EXPECT_EQ(8, sizeof(facade));
    EXPECT_EQ(SimpleData.size(), facade.length());
    EXPECT_EQ(0x1111, facade.first());
    EXPECT_EQ(0x2222, facade.second());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second] = facade;

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second);
#   endif
}

TEST(Dissect_TestCase, Complex)
{
    ComplexCFacade facade(ComplexData.data());

    EXPECT_EQ(8, sizeof(facade));
    EXPECT_EQ(ComplexData.size(), facade.length());
    EXPECT_EQ(0x1111, facade.first());
    EXPECT_EQ(0x2222, facade.second().first());
    EXPECT_EQ(0x3333, facade.second().second());
    EXPECT_EQ(0x4444, facade.third());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = facade;

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second.first());
    EXPECT_EQ(0x3333, second.second());
    EXPECT_EQ(0x4444, third);
#   endif
}

TEST(Dissect_TestCase, VeryComplex)
{
    VeryComplexCFacade facade(VeryComplexData.data());

    EXPECT_EQ(8, sizeof(facade));
    EXPECT_EQ(VeryComplexData.size(), facade.length());
    EXPECT_EQ(0x1111, facade.first());
    EXPECT_EQ(0x2222, facade.second().first());
    EXPECT_EQ(0x3333, facade.second().second().first());
    EXPECT_EQ(0x4444, facade.second().second().second());
    EXPECT_EQ(0x5555, facade.second().third());
    EXPECT_EQ(0x6666, facade.third());

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second, third] = facade;

    EXPECT_EQ(0x1111, first);
    EXPECT_EQ(0x2222, second.first());
    EXPECT_EQ(0x3333, second.second().first());
    EXPECT_EQ(0x4444, second.second().second());
    EXPECT_EQ(0x5555, second.third());
    EXPECT_EQ(0x6666, third);
#   endif
}

TEST(Assignment_TestCase, BasicAssignBare)
{
    block<uint32_t> src, dst;

    src.set<0>(0x1111);

    dst = src;

    memset(src.data(), 0, src.length());

    EXPECT_EQ(0x1111, dst.get<0>());
}

TEST(Assignment_TestCase, BasicMoveBare)
{
    block<uint32_t> src, dst;

    src.set<0>(0x1111);

    dst = move(src);

    memset(src.data(), 0, src.length());

    EXPECT_EQ(0x1111, dst.get<0>());
}

TEST(Assignment_TestCase, BasicAssignWrapped)
{
    auto src = IntPairBlock().first(0x1111).second(0x2222);

    IntPairBlock dst;

    dst = src;

    memset(src.data(), 0, src.length());

    EXPECT_EQ(0x1111, dst.first());
    EXPECT_EQ(0x2222, dst.second());
}

TEST(Assignment_TestCase, BasicMoveWrapped)
{
    auto src = IntPairBlock().first(0x1111).second(0x2222);

    IntPairBlock dst;

    dst = move(src);

    memset(src.data(), 0, src.length());

    EXPECT_EQ(0x1111, dst.first());
    EXPECT_EQ(0x2222, dst.second());
}

struct int_struct { int value; };

TEST(Misc_TestCase, BasicAggregatable)
{
    block<int_struct, int_struct> block;

    block.set<0>(0x1111);
    block.set<1>(0x2222);

    EXPECT_EQ(16, sizeof(block)); // length() + 8
    EXPECT_EQ(8, block.length()); // 4 + 4
    EXPECT_EQ(0x1111, block.get<0>().value);
    EXPECT_EQ(0x2222, block.get<1>().value);

#   ifdef TEST_STRUCTURED_BINDING
    auto [first, second] = block;

    memset(block.data(), 0, block.length());

    EXPECT_EQ(0x1111, first.value);
    EXPECT_EQ(0x2222, second.value);
#   endif
}
