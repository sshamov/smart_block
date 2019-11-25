#include <gtest/gtest.h>

#include "../../block.h"
#include "../../builder.h"
#include "../../std/variant.h"
#include "../../std/string.h"

#include "../aggregated_visitor.h"
#include "../test_utils.h"
#include "variant_test_data.h"

using namespace std;
using namespace smart_block;

struct int_struct { int value; };

ostream& operator <<(ostream& os, const int_struct& val)
{
    os << "int_struct(";

    if (os.flags() & std::ios_base::hex) os << "0x";

    return os << val.value << ')';
}

TEST(Variant_TestCase, Simple1)
{
    block<variant<uint32_t, int_struct>> block;

    EXPECT_EQ(16, sizeof(block)); // length() + 8
    EXPECT_EQ(8, block.length()); // 4 + 1 + padding3

    block.edit<0>().as<uint32_t>() = 0x1111;

    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<uint32_t>());

    auto visitor = make_visitor()
        .add([](const uint32_t&   v) { return pair(0, v); })
        .add([](const int_struct& v) { return pair(1, static_cast<uint32_t>(v.value)); })
        .done();

    auto [index, value] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0x1111, value);

    EXPECT_EQ(VariantSimple1Str, to_string(block));
    EXPECT_EQ(VariantSimple1Data, block.span());

    block.edit<0>().as<int_struct>() = { 0x1111 };

    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<int_struct>().value);

    tie(index, value) = pair(99, 0);
    tie(index, value) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0x1111, value);

    EXPECT_EQ(VariantSimple1Str2, to_string(block));
    EXPECT_EQ(VariantSimple1Data2, block.span());
}

TEST(Variant_TestCase, Simple2)
{
    builder<variant<uint32_t, uint64_t>> builder;

    EXPECT_EQ(16, sizeof(builder)); // 8 + 1 + padding7

    builder.edit<0>().as<uint32_t>() = 0x1111;

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(6, block.length()); // 1 + (1 + 4)
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<uint32_t>());

    auto visitor = make_visitor()
        .add([](const uint32_t& v) { return pair(0, static_cast<uint64_t>(v)); })
        .add([](const uint64_t& v) { return pair(1, v); })
        .done();

    auto [index, value] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0x1111, value);

    EXPECT_EQ(VariantSimple2Str, to_string(block));
    EXPECT_EQ(VariantSimple2Data, block.span());

    builder.edit<0>().as<uint64_t>() = 0x2222;

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(10, block.length()); // 1 + (1 + 8)
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ(0x2222, block.get<0>().as<uint64_t>());

    tie(index, value) = pair(99, 0);
    tie(index, value) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0x2222, value);

    EXPECT_EQ(VariantSimple2Str2, to_string(block));
    EXPECT_EQ(VariantSimple2Data2, block.span());
}

TEST(Variant_TestCase, Simple3)
{
    builder<variant<uint32_t, string>> builder;

    EXPECT_EQ(40, sizeof(builder)); // 32 + 1 + padding7

    builder.edit<0>().as<uint32_t>() = 0x1111;

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(6, block.length()); // 1 + (1 + 4)
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<uint32_t>());

    auto visitor = make_visitor()
        .add([](const uint32_t& v) { return tuple(0, v, ""s); })
        .add([](const string&   v) { return tuple(1, 0u, v); })
        .done();

    auto [index, value1, value2] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0x1111, value1);
    EXPECT_EQ("", value2);

    EXPECT_EQ(VariantSimple3Str, to_string(block));
    EXPECT_EQ(VariantSimple3Data, block.span());

    builder.edit<0>().as<string>() = "abcdefghijklmnopqrstuvwxyz";

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(29, block.length()); // 1 + (1 + 26 + 1)
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.get<0>().as<string>());

    tie(index, value1, value2) = tuple(99, 0, "");
    tie(index, value1, value2) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0, value1);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", value2);

    EXPECT_EQ(VariantSimple3Str2, to_string(block));
    EXPECT_EQ(VariantSimple3Data2, block.span());
}

TEST(Variant_TestCase, Simple4)
{
    builder<variant<string, uint32_t>> builder;

    EXPECT_EQ(40, sizeof(builder)); // 32 + 1 + padding7

    builder.edit<0>().as<string>() = "abcdefghijklmnopqrstuvwxyz";

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(29, block.length()); // 1 + (1 + 26 + 1)
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.get<0>().as<string>());

    auto visitor = make_visitor()
        .add([](const string&   v) { return tuple(0, v, 0u); })
        .add([](const uint32_t& v) { return tuple(1, ""s, v); })
        .done();

    auto [index, value1, value2] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", value1);
    EXPECT_EQ(0, value2);

    EXPECT_EQ(VariantSimple4Str, to_string(block));
    EXPECT_EQ(VariantSimple4Data, block.span());

    builder.edit<0>().as<uint32_t>() = 0x1111;

    block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(6, block.length()); // 1 + (1 + 4)
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<uint32_t>());

    tie(index, value1, value2) = tuple(99, "", 0);
    tie(index, value1, value2) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ("", value1);
    EXPECT_EQ(0x1111, value2);

    EXPECT_EQ(VariantSimple4Str2, to_string(block));
    EXPECT_EQ(VariantSimple4Data2, block.span());
}

TEST(Variant_TestCase, Monostate1)
{
    builder<variant<monostate, uint32_t, uint64_t>> builder;

    EXPECT_EQ(16, sizeof(builder)); // 8 + 1 + padding7

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(1, block.length());
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(monostate(), block.get<0>().as<monostate>());

    auto visitor = make_visitor()
        .add([](const monostate&)  { return pair(0, 0ul); })
        .add([](const uint32_t& v) { return pair(1, static_cast<uint64_t>(v)); })
        .add([](const uint64_t& v) { return pair(2, v); })
        .done();

    auto [index, value] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0, value);

    EXPECT_EQ(VariantMonostate1Str, to_string(block));
    EXPECT_EQ(VariantMonostate1Data, block.span());

    builder.edit<0>().as<uint32_t>() = 0x1111;

    block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(6, block.length()); // 1 + (1 + 4)
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<uint32_t>());

    tie(index, value) = pair(99, 0);
    tie(index, value) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0x1111, value);

    EXPECT_EQ(VariantMonostate1Str2, to_string(block));
    EXPECT_EQ(VariantMonostate1Data2, block.span());

    builder.edit<0>().as<uint64_t>() = 0x2222;

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(10, block.length()); // 1 + (1 + 8)
    EXPECT_EQ(2, block.get<0>().index());
    EXPECT_EQ(0x2222, block.get<0>().as<uint64_t>());

    tie(index, value) = pair(99, 0);
    tie(index, value) = block.get<0>().apply(visitor);

    EXPECT_EQ(2, index);
    EXPECT_EQ(0x2222, value);

    EXPECT_EQ(VariantMonostate1Str3, to_string(block));
    EXPECT_EQ(VariantMonostate1Data3, block.span());
}

TEST(Variant_TestCase, Monostate2)
{
    builder<variant<monostate, uint32_t, string>> builder;

    EXPECT_EQ(40, sizeof(builder)); // 32 + 1 + padding7

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(1, block.length());
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(monostate(), block.get<0>().as<monostate>());

    auto visitor = make_visitor()
        .add([](const monostate&)  { return tuple(0, 0u, ""s); })
        .add([](const uint32_t& v) { return tuple(1, v, ""s); })
        .add([](const string&   v) { return tuple(2, 0u, v); })
        .done();

    auto [index, value1, value2] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0, value1);
    EXPECT_EQ("", value2);

    EXPECT_EQ(VariantMonostate2Str, to_string(block));
    EXPECT_EQ(VariantMonostate2Data, block.span());

    builder.edit<0>().as<uint32_t>() = 0x1111;

    block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(6, block.length()); // 1 + (1 + 4)
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<uint32_t>());

    tie(index, value1, value2) = tuple(99, 0, "");
    tie(index, value1, value2) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0x1111, value1);
    EXPECT_EQ("", value2);

    EXPECT_EQ(VariantMonostate2Str2, to_string(block));
    EXPECT_EQ(VariantMonostate2Data2, block.span());

    builder.edit<0>().as<string>() = "abcdefghijklmnopqrstuvwxyz";

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(29, block.length()); // 1 + (1 + 26 + 1)
    EXPECT_EQ(2, block.get<0>().index());
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.get<0>().as<string>());

    tie(index, value1, value2) = tuple(99, 0, "");
    tie(index, value1, value2) = block.get<0>().apply(visitor);

    EXPECT_EQ(2, index);
    EXPECT_EQ(0, value1);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", value2);

    EXPECT_EQ(VariantMonostate2Str3, to_string(block));
    EXPECT_EQ(VariantMonostate2Data3, block.span());
}

TEST(Variant_TestCase, Monostate3)
{
    builder<variant<monostate, string, uint32_t>> builder;

    EXPECT_EQ(40, sizeof(builder)); // 32 + 1 + padding7

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(1, block.length());
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(monostate(), block.get<0>().as<monostate>());

    auto visitor = make_visitor()
        .add([](const monostate&)  { return tuple(0, ""s, 0u); })
        .add([](const string&   v) { return tuple(1, v, 0u); })
        .add([](const uint32_t& v) { return tuple(2, ""s, v); })
        .done();

    auto [index, value1, value2] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ("", value1);
    EXPECT_EQ(0, value2);

    EXPECT_EQ(VariantMonostate3Str, to_string(block));
    EXPECT_EQ(VariantMonostate3Data, block.span());

    builder.edit<0>().as<string>() = "abcdefghijklmnopqrstuvwxyz";

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(29, block.length()); // 1 + (1 + 26 + 1)
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.get<0>().as<string>());

    tie(index, value1, value2) = tuple(99, "", 0);
    tie(index, value1, value2) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", value1);
    EXPECT_EQ(0, value2);

    EXPECT_EQ(VariantMonostate3Str2, to_string(block));
    EXPECT_EQ(VariantMonostate3Data2, block.span());

    builder.edit<0>().as<uint32_t>() = 0x1111;

    block = builder.build();

    EXPECT_EQ(16, sizeof(block)); // 8 + 8
    EXPECT_EQ(6, block.length()); // 1 + (1 + 4)
    EXPECT_EQ(2, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<uint32_t>());

    tie(index, value1, value2) = tuple(99, "", 0);
    tie(index, value1, value2) = block.get<0>().apply(visitor);

    EXPECT_EQ(2, index);
    EXPECT_EQ("", value1);
    EXPECT_EQ(0x1111, value2);

    EXPECT_EQ(VariantMonostate3Str3, to_string(block));
    EXPECT_EQ(VariantMonostate3Data3, block.span());
}

TEST(Variant_TestCase, Block1)
{
    using block1 = block<uint32_t, uint64_t>;
    using block2 = block<uint64_t, uint32_t>;

    block<variant<block1, block2>> block;

    EXPECT_EQ(24, sizeof(block));  // length() + padding3 + 8
    EXPECT_EQ(13, block.length()); // (4 + 8) + 1

    block.edit<0>().as<block1>().set<0>(0x1111);
    block.edit<0>().as<block1>().set<1>(0x2222);

    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<block1>().get<0>());
    EXPECT_EQ(0x2222, block.get<0>().as<block1>().get<1>());

    using cfacade1 = block_cfacade<uint32_t, uint64_t>;
    using cfacade2 = block_cfacade<uint64_t, uint32_t>;

    auto visitor = make_visitor()
        .add([](const cfacade1& v) { return tuple(0, static_cast<uint64_t>(v.get<0>()), v.get<1>()); })
        .add([](const cfacade2& v) { return tuple(1, v.get<0>(), static_cast<uint64_t>(v.get<1>())); })
        .done();

    auto [index, value1, value2] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0x1111, value1);
    EXPECT_EQ(0x2222, value2);

    EXPECT_EQ(VariantBlock1Str, to_string(block));
    EXPECT_EQ(VariantBlock1Data, block.span());

    block.edit<0>().as<block2>().set<0>(0x1111);
    block.edit<0>().as<block2>().set<1>(0x2222);

    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<block2>().get<0>());
    EXPECT_EQ(0x2222, block.get<0>().as<block2>().get<1>());

    tie(index, value1, value2) = tuple(99, 0, 0);
    tie(index, value1, value2) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0x1111, value1);
    EXPECT_EQ(0x2222, value2);

    EXPECT_EQ(VariantBlock1Str2, to_string(block));
    EXPECT_EQ(VariantBlock1Data2, block.span());
}

TEST(Variant_TestCase, Block2)
{
    using block1 = block<uint32_t, uint32_t>;
    using block2 = block<uint64_t, uint64_t>;

    builder<variant<block1, block2>> builder;

    EXPECT_EQ(17, sizeof(builder)); // (8 + 8) + 1

    builder.edit<0>().as<block1>().set<0>(0x1111);
    builder.edit<0>().as<block1>().set<1>(0x2222);

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(10, block.length()); // 1 + (1 + (4 + 4))
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<block1>().get<0>());
    EXPECT_EQ(0x2222, block.get<0>().as<block1>().get<1>());

    using cfacade1 = block_cfacade<uint32_t, uint32_t>;
    using cfacade2 = block_cfacade<uint64_t, uint64_t>;

    auto visitor = make_visitor()
        .add([](const cfacade1& v) {
            return tuple(0,
                static_cast<uint64_t>(v.get<0>()),
                static_cast<uint64_t>(v.get<1>()));
        })
        .add([](const cfacade2& v) { return tuple(1, v.get<0>(), v.get<1>()); })
        .done();

    auto [index, value1, value2] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0x1111, value1);
    EXPECT_EQ(0x2222, value2);

    EXPECT_EQ(VariantBlock2Str, to_string(block));
    EXPECT_EQ(VariantBlock2Data, block.span());

    builder.edit<0>().as<block2>().set<0>(0x1111);
    builder.edit<0>().as<block2>().set<1>(0x2222);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(18, block.length()); // 1 + (1 + (8 + 8))
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<block2>().get<0>());
    EXPECT_EQ(0x2222, block.get<0>().as<block2>().get<1>());

    tie(index, value1, value2) = tuple(99, 0, 0);
    tie(index, value1, value2) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0x1111, value1);
    EXPECT_EQ(0x2222, value2);

    EXPECT_EQ(VariantBlock2Str2, to_string(block));
    EXPECT_EQ(VariantBlock2Data2, block.span());
}

TEST(Variant_TestCase, Block3)
{
    using block1 = block<uint32_t, uint32_t>;
    using block2 = block<string, string>;

    builder<variant<block1, block2>> builder;

    EXPECT_EQ(65, sizeof(builder)); // 32 + 32 + 1

    builder.edit<0>().as<block1>().set<0>(0x1111);
    builder.edit<0>().as<block1>().set<1>(0x2222);

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(10, block.length()); // 1 + (1 + (4 + 4))
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<block1>().get<0>());
    EXPECT_EQ(0x2222, block.get<0>().as<block1>().get<1>());

    using cfacade1 = block_cfacade<uint32_t, uint32_t>;
    using cfacade2 = block_cfacade<string, string>;

    auto visitor = make_visitor()
        .add([](const cfacade1& v) { return tuple(0, v.get<0>(), v.get<1>(), ""s, ""s); })
        .add([](const cfacade2& v) { return tuple(1, 0u, 0u, v.get<0>(), v.get<1>()); })
        .done();

    auto [index, int1, int2, str1, str2] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0x1111, int1);
    EXPECT_EQ(0x2222, int2);
    EXPECT_EQ("", str1);
    EXPECT_EQ("", str2);

    EXPECT_EQ(VariantBlock3Str, to_string(block));
    EXPECT_EQ(VariantBlock3Data, block.span());

    builder.edit<0>().as<block2>().set<0>("abcdefghijklm");
    builder.edit<0>().as<block2>().set<1>("nopqrstuvwxyz");

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(32, block.length()); // 1 + (1 + (2 + (13 + 1) + (13 + 1)))
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ("abcdefghijklm", block.get<0>().as<block2>().get<0>());
    EXPECT_EQ("nopqrstuvwxyz", block.get<0>().as<block2>().get<1>());

    tie(index, int1, int2, str1, str2) = tuple(99, 0, 0, "", "");
    tie(index, int1, int2, str1, str2) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0, int1);
    EXPECT_EQ(0, int2);
    EXPECT_EQ("abcdefghijklm", str1);
    EXPECT_EQ("nopqrstuvwxyz", str2);

    EXPECT_EQ(VariantBlock3Str2, to_string(block));
    EXPECT_EQ(VariantBlock3Data2, block.span());
}

TEST(Variant_TestCase, Block4)
{
    using block1 = block<string, string>;
    using block2 = block<uint32_t, uint32_t>;

    builder<variant<block1, block2>> builder;

    EXPECT_EQ(65, sizeof(builder)); // 32 + 32 + 1

    builder.edit<0>().as<block1>().set<0>("abcdefghijklm");
    builder.edit<0>().as<block1>().set<1>("nopqrstuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(32, block.length()); // 1 + (1 + (2 + (13 + 1) + (13 + 1)))
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ("abcdefghijklm", block.get<0>().as<block1>().get<0>());
    EXPECT_EQ("nopqrstuvwxyz", block.get<0>().as<block1>().get<1>());

    using cfacade1 = block_cfacade<string, string>;
    using cfacade2 = block_cfacade<uint32_t, uint32_t>;

    auto visitor = make_visitor()
        .add([](const cfacade1& v) { return tuple(0, 0u, 0u, v.get<0>(), v.get<1>()); })
        .add([](const cfacade2& v) { return tuple(1, v.get<0>(), v.get<1>(), ""s, ""s); })
        .done();

    auto [index, int1, int2, str1, str2] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0, int1);
    EXPECT_EQ(0, int2);
    EXPECT_EQ("abcdefghijklm", str1);
    EXPECT_EQ("nopqrstuvwxyz", str2);

    EXPECT_EQ(VariantBlock4Str, to_string(block));
    EXPECT_EQ(VariantBlock4Data, block.span());

    builder.edit<0>().as<block2>().set<0>(0x1111);
    builder.edit<0>().as<block2>().set<1>(0x2222);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(10, block.length()); // 1 + (1 + (4 + 4))
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<block2>().get<0>());
    EXPECT_EQ(0x2222, block.get<0>().as<block2>().get<1>());

    tie(index, int1, int2, str1, str2) = tuple(99, 0, 0, "", "");
    tie(index, int1, int2, str1, str2) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0x1111, int1);
    EXPECT_EQ(0x2222, int2);
    EXPECT_EQ("", str1);
    EXPECT_EQ("", str2);

    EXPECT_EQ(VariantBlock4Str2, to_string(block));
    EXPECT_EQ(VariantBlock4Data2, block.span());
}

TEST(Variant_TestCase, Block5)
{
    using block1 = block<uint32_t, string>;
    using block2 = block<string, uint32_t>;

    builder<variant<block1, block2>> builder;

    EXPECT_EQ(37, sizeof(builder)); // 32 + 4 + 1

    builder.edit<0>().as<block1>().set<0>(0x1111);
    builder.edit<0>().as<block1>().set<1>("abcdefghijklmnopqrstuvwxyz");

    auto block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(34, block.length()); // 1 + (1 + (1 + 4 + (26 + 1)))
    EXPECT_EQ(0, block.get<0>().index());
    EXPECT_EQ(0x1111, block.get<0>().as<block1>().get<0>());
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.get<0>().as<block1>().get<1>());

    using cfacade1 = block_cfacade<uint32_t, string>;
    using cfacade2 = block_cfacade<string, uint32_t>;

    auto visitor = make_visitor()
        .add([](const cfacade1& v) { return tuple(0, v.get<0>(), v.get<1>()); })
        .add([](const cfacade2& v) { return tuple(1, v.get<1>(), v.get<0>()); })
        .done();

    auto [index, value1, value2] = block.get<0>().apply(visitor);

    EXPECT_EQ(0, index);
    EXPECT_EQ(0x1111, value1);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", value2);

    EXPECT_EQ(VariantBlock5Str, to_string(block));
    EXPECT_EQ(VariantBlock5Data, block.span());

    builder.edit<0>().as<block2>().set<0>("abcdefghijklmnopqrstuvwxyz");
    builder.edit<0>().as<block2>().set<1>(0x1111);

    block = builder.build();

    EXPECT_EQ(16, sizeof(block));  // 8 + 8
    EXPECT_EQ(34, block.length()); // 1 + (1 + (1 + (26 + 1) + 4))
    EXPECT_EQ(1, block.get<0>().index());
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", block.get<0>().as<block2>().get<0>());
    EXPECT_EQ(0x1111, block.get<0>().as<block2>().get<1>());

    tie(index, value1, value2) = tuple(99, 0, "");
    tie(index, value1, value2) = block.get<0>().apply(visitor);

    EXPECT_EQ(1, index);
    EXPECT_EQ(0x1111, value1);
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", value2);

    EXPECT_EQ(VariantBlock5Str2, to_string(block));
    EXPECT_EQ(VariantBlock5Data2, block.span());
}
