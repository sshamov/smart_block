#ifndef SKIP_SHARED
#include "profile_impl.h"
#include "consume.h"

template <typename T> void destruct(void*) { }
template <typename T> void copy(void*, void*) { }
template <typename T> void move(void*, void*) { }
#endif

// basic_simple_block

template <>
void construct<basic_simple_block>(void* obj)
{
    basic_simple_block& block = *static_cast<basic_simple_block*>(new (obj) basic_simple_block);

    block.set<0>(0x1111);
    block.set<1>(0x2222);
}

template <>
void dissect<basic_simple_block>(void* obj)
{
    basic_simple_block& block = *static_cast<basic_simple_block*>(obj);

    consume_cref(block.get<0>());
    consume_cref(block.get<1>());
}

template void destruct<basic_simple_block>(void*);
template void copy<basic_simple_block>(void*, void*);
template void move<basic_simple_block>(void*, void*);

// basic_complex_block

template <>
void construct<basic_complex_block>(void* obj)
{
    basic_complex_block& block = *static_cast<basic_complex_block*>(new (obj) basic_complex_block);

    block.set<0>(0x1111);
    block.sub<1>().set<0>(0x2222);
    block.sub<1>().set<1>(0x3333);
    block.set<2>(0x4444);
}

template <>
void dissect<basic_complex_block>(void* obj)
{
    basic_complex_block& block = *static_cast<basic_complex_block*>(obj);

    consume_cref(block.get<0>());
    consume_cref(block.sub<1>().get<0>());
    consume_cref(block.sub<1>().get<1>());
    consume_cref(block.get<2>());
}

template void destruct<basic_complex_block>(void*);
template void copy<basic_complex_block>(void*, void*);
template void move<basic_complex_block>(void*, void*);

// basic_very_complex_block

template <>
void construct<basic_very_complex_block>(void* obj)
{
    basic_very_complex_block& block = *static_cast<basic_very_complex_block*>(new (obj) basic_very_complex_block);

    block.set<0>(0x1111);
    block.sub<1>().set<0>(0x2222);
    block.sub<1>().sub<1>().set<0>(0x3333);
    block.sub<1>().sub<1>().set<1>(0x4444);
    block.sub<1>().set<2>(0x5555);
    block.set<2>(0x6666);
}

template <>
void dissect<basic_very_complex_block>(void* obj)
{
    basic_very_complex_block& block = *static_cast<basic_very_complex_block*>(obj);

    consume_cref(block.get<0>());
    consume_cref(block.sub<1>().get<0>());
    consume_cref(block.sub<1>().sub<1>().get<0>());
    consume_cref(block.sub<1>().sub<1>().get<1>());
    consume_cref(block.sub<1>().get<2>());
    consume_cref(block.get<2>());
}

template void destruct<basic_very_complex_block>(void*);
template void copy<basic_very_complex_block>(void*, void*);
template void move<basic_very_complex_block>(void*, void*);

// string_simple1_block

template <>
void construct<string_simple1_block>(void* obj)
{
    string_simple1_block::builder builder;

    builder.set<0>("abcdefghijklmnopqrstuvwxyz");
    builder.set<1>(0x1111);
    builder.set<2>("abcdefghijklmnopqrstuvwxyz");

    new (obj) string_simple1_block(builder.build());
}

template <>
void dissect<string_simple1_block>(void* obj)
{
    string_simple1_block& block = *static_cast<string_simple1_block*>(obj);

    consume_cref(block.get<0>());
    consume_cref(block.get<1>());
    consume_cref(block.get<2>());
}

template void destruct<string_simple1_block>(void*);
template void copy<string_simple1_block>(void*, void*);
template void move<string_simple1_block>(void*, void*);

// string_simple2_block

template <>
void construct<string_simple2_block>(void* obj)
{
    string_simple2_block::builder builder;

    builder.set<0>(0x1111);
    builder.set<1>("abcdefghijklmnopqrstuvwxyz");
    builder.set<2>(0x2222);

    new (obj) string_simple2_block(builder.build());
}

template <>
void dissect<string_simple2_block>(void* obj)
{
    string_simple2_block& block = *static_cast<string_simple2_block*>(obj);

    consume_cref(block.get<0>());
    consume_cref(block.get<1>());
    consume_cref(block.get<2>());
}

template void destruct<string_simple2_block>(void*);
template void copy<string_simple2_block>(void*, void*);
template void move<string_simple2_block>(void*, void*);

// string_complex1_block

template <>
void construct<string_complex1_block>(void* obj)
{
    string_complex1_block::builder builder;

    builder.set<0>("abcdefghijklmnopqrstuvwxyz");
    builder.sub<1>().set<0>(0x1111);
    builder.sub<1>().set<1>(0x2222);
    builder.set<2>("abcdefghijklmnopqrstuvwxyz");

    new (obj) string_complex1_block(builder.build());
}

template <>
void dissect<string_complex1_block>(void* obj)
{
    string_complex1_block& block = *static_cast<string_complex1_block*>(obj);

    consume_cref(block.get<0>());
    consume_cref(block.sub<1>().get<0>());
    consume_cref(block.sub<1>().get<1>());
    consume_cref(block.get<2>());
}

template void destruct<string_complex1_block>(void*);
template void copy<string_complex1_block>(void*, void*);
template void move<string_complex1_block>(void*, void*);

// string_complex2_block

template <>
void construct<string_complex2_block>(void* obj)
{
    string_complex2_block::builder builder;

    builder.set<0>(0x1111);
    builder.sub<1>().set<0>("abcdefghijklmnopqrstuvwxyz");
    builder.sub<1>().set<1>("abcdefghijklmnopqrstuvwxyz");
    builder.set<2>(0x2222);

    new (obj) string_complex2_block(builder.build());
}

template <>
void dissect<string_complex2_block>(void* obj)
{
    string_complex2_block& block = *static_cast<string_complex2_block*>(obj);

    consume_cref(block.get<0>());
    consume_cref(block.sub<1>().get<0>());
    consume_cref(block.sub<1>().get<1>());
    consume_cref(block.get<2>());
}

template void destruct<string_complex2_block>(void*);
template void copy<string_complex2_block>(void*, void*);
template void move<string_complex2_block>(void*, void*);

// string_very_complex1_block

template <>
void construct<string_very_complex1_block>(void* obj)
{
    string_very_complex1_block::builder builder;

    builder.set<0>("abcdefghijklmnopqrstuvwxyz");
    builder.sub<1>().set<0>(0x1111);
    builder.sub<1>().sub<1>().set<0>("abcdefghijklmnopqrstuvwxyz");
    builder.sub<1>().sub<1>().set<1>("abcdefghijklmnopqrstuvwxyz");
    builder.sub<1>().set<2>(0x2222);
    builder.set<2>("abcdefghijklmnopqrstuvwxyz");

    new (obj) string_very_complex1_block(builder.build());
}

template <>
void dissect<string_very_complex1_block>(void* obj)
{
    string_very_complex1_block& block = *static_cast<string_very_complex1_block*>(obj);

    consume_cref(block.get<0>());
    consume_cref(block.sub<1>().get<0>());
    consume_cref(block.sub<1>().sub<1>().get<0>());
    consume_cref(block.sub<1>().sub<1>().get<1>());
    consume_cref(block.sub<1>().get<2>());
    consume_cref(block.get<2>());
}

template void destruct<string_very_complex1_block>(void*);
template void copy<string_very_complex1_block>(void*, void*);
template void move<string_very_complex1_block>(void*, void*);

// string_very_complex2_block

template <>
void construct<string_very_complex2_block>(void* obj)
{
    string_very_complex2_block::builder builder;

    builder.set<0>(0x1111);
    builder.sub<1>().set<0>("abcdefghijklmnopqrstuvwxyz");
    builder.sub<1>().sub<1>().set<0>(0x2222);
    builder.sub<1>().sub<1>().set<1>(0x3333);
    builder.sub<1>().set<2>("abcdefghijklmnopqrstuvwxyz");
    builder.set<2>(0x4444);

    new (obj) string_very_complex2_block(builder.build());
}

template <>
void dissect<string_very_complex2_block>(void* obj)
{
    string_very_complex2_block& block = *static_cast<string_very_complex2_block*>(obj);

    consume_cref(block.get<0>());
    consume_cref(block.sub<1>().get<0>());
    consume_cref(block.sub<1>().sub<1>().get<0>());
    consume_cref(block.sub<1>().sub<1>().get<1>());
    consume_cref(block.sub<1>().get<2>());
    consume_cref(block.get<2>());
}

template void destruct<string_very_complex2_block>(void*);
template void copy<string_very_complex2_block>(void*, void*);
template void move<string_very_complex2_block>(void*, void*);
