#ifndef SKIP_SHARED
#include "profile_impl.h"
#include "consume.h"

template <typename T> void destruct(void*) { }
template <typename T> void copy(void*, void*) { }
template <typename T> void move(void*, void*) { }
#endif

// simple_struct

template <>
void construct<basic_simple_struct>(void* obj)
{
    basic_simple_struct& s = *static_cast<basic_simple_struct*>(new (obj) basic_simple_struct);

    s.value1 = 0x1111;
    s.value2 = 0x2222;
}

template <>
void dissect<basic_simple_struct>(void* obj)
{
    basic_simple_struct& s = *static_cast<basic_simple_struct*>(obj);

    consume_cref(s.value1);
    consume_cref(s.value2);
}

template void destruct<basic_simple_struct>(void*);
template void copy<basic_simple_struct>(void*, void*);
template void move<basic_simple_struct>(void*, void*);

// complex_struct

template <>
void construct<basic_complex_struct>(void* obj)
{
    basic_complex_struct& s = *static_cast<basic_complex_struct*>(new (obj) basic_complex_struct);

    s.value1 = 0x1111;
    s.sub2.value1 = 0x2222;
    s.sub2.value2 = 0x3333;
    s.value3 = 0x4444;
}

template <>
void dissect<basic_complex_struct>(void* obj)
{
    basic_complex_struct& s = *static_cast<basic_complex_struct*>(obj);

    consume_cref(s.value1);
    consume_cref(s.sub2.value1);
    consume_cref(s.sub2.value2);
    consume_cref(s.value3);
}

template void destruct<basic_complex_struct>(void*);
template void copy<basic_complex_struct>(void*, void*);
template void move<basic_complex_struct>(void*, void*);

// very_complex_struct

template <>
void construct<basic_very_complex_struct>(void* obj)
{
    basic_very_complex_struct& s = *static_cast<basic_very_complex_struct*>(new (obj) basic_very_complex_struct);

    s.value1 = 0x1111;
    s.sub2.value1 = 0x2222;
    s.sub2.sub2.value1 = 0x3333;
    s.sub2.sub2.value2 = 0x4444;
    s.sub2.value3 = 0x5555;
    s.value3 = 0x6666;
}

template <>
void dissect<basic_very_complex_struct>(void* obj)
{
    basic_very_complex_struct& s = *static_cast<basic_very_complex_struct*>(obj);

    consume_cref(s.value1);
    consume_cref(s.sub2.value1);
    consume_cref(s.sub2.sub2.value1);
    consume_cref(s.sub2.sub2.value2);
    consume_cref(s.sub2.value3);
    consume_cref(s.value3);
}

template void destruct<basic_very_complex_struct>(void*);
template void copy<basic_very_complex_struct>(void*, void*);
template void move<basic_very_complex_struct>(void*, void*);

// string_simple1_struct

template <>
void construct<string_simple1_struct>(void* obj)
{
    string_simple1_struct& s = *static_cast<string_simple1_struct*>(new (obj) string_simple1_struct);

    s.value1 = "abcdefghijklmnopqrstuvwxyz";
    s.value2 = 0x1111;
    s.value3 = "abcdefghijklmnopqrstuvwxyz";
}

template <>
void dissect<string_simple1_struct>(void* obj)
{
    string_simple1_struct& s = *static_cast<string_simple1_struct*>(obj);

    consume_cref(s.value1);
    consume_cref(s.value2);
    consume_cref(s.value3);
}

template void destruct<string_simple1_struct>(void*);
template void copy<string_simple1_struct>(void*, void*);
template void move<string_simple1_struct>(void*, void*);

// string_simple2_struct

template <>
void construct<string_simple2_struct>(void* obj)
{
    string_simple2_struct& s = *static_cast<string_simple2_struct*>(new (obj) string_simple2_struct);

    s.value1 = 0x1111;
    s.value2 = "abcdefghijklmnopqrstuvwxyz";
    s.value3 = 0x2222;
}

template <>
void dissect<string_simple2_struct>(void* obj)
{
    string_simple2_struct& s = *static_cast<string_simple2_struct*>(obj);

    consume_cref(s.value1);
    consume_cref(s.value2);
    consume_cref(s.value3);
}

template void destruct<string_simple2_struct>(void*);
template void copy<string_simple2_struct>(void*, void*);
template void move<string_simple2_struct>(void*, void*);

// string_complex1_struct

template <>
void construct<string_complex1_struct>(void* obj)
{
    string_complex1_struct& s = *static_cast<string_complex1_struct*>(new (obj) string_complex1_struct);

    s.value1 = "abcdefghijklmnopqrstuvwxyz";
    s.sub2.value1 = 0x1111;
    s.sub2.value2 = 0x2222;
    s.value3 = "abcdefghijklmnopqrstuvwxyz";
}

template <>
void dissect<string_complex1_struct>(void* obj)
{
    string_complex1_struct& s = *static_cast<string_complex1_struct*>(obj);

    consume_cref(s.value1);
    consume_cref(s.sub2.value1);
    consume_cref(s.sub2.value2);
    consume_cref(s.value3);
}

template void destruct<string_complex1_struct>(void*);
template void copy<string_complex1_struct>(void*, void*);
template void move<string_complex1_struct>(void*, void*);

// string_complex2_struct

template <>
void construct<string_complex2_struct>(void* obj)
{
    string_complex2_struct& s = *static_cast<string_complex2_struct*>(new (obj) string_complex2_struct);

    s.value1 = 0x1111;
    s.sub2.value1 = "abcdefghijklmnopqrstuvwxyz";
    s.sub2.value2 = "abcdefghijklmnopqrstuvwxyz";
    s.value3 = 0x2222;
}

template <>
void dissect<string_complex2_struct>(void* obj)
{
    string_complex2_struct& s = *static_cast<string_complex2_struct*>(obj);

    consume_cref(s.value1);
    consume_cref(s.sub2.value1);
    consume_cref(s.sub2.value2);
    consume_cref(s.value3);
}

template void destruct<string_complex2_struct>(void*);
template void copy<string_complex2_struct>(void*, void*);
template void move<string_complex2_struct>(void*, void*);

// string_very_complex1_struct

template <>
void construct<string_very_complex1_struct>(void* obj)
{
    string_very_complex1_struct& s =
        *static_cast<string_very_complex1_struct*>(new (obj) string_very_complex1_struct);

    s.value1 = "abcdefghijklmnopqrstuvwxyz";
    s.sub2.value1 = 0x1111;
    s.sub2.sub2.value1 = "abcdefghijklmnopqrstuvwxyz";
    s.sub2.sub2.value2 = "abcdefghijklmnopqrstuvwxyz";
    s.sub2.value3 = 0x2222;
    s.value3 = "abcdefghijklmnopqrstuvwxyz";
}

template <>
void dissect<string_very_complex1_struct>(void* obj)
{
    string_very_complex1_struct& s = *static_cast<string_very_complex1_struct*>(obj);

    consume_cref(s.value1);
    consume_cref(s.sub2.value1);
    consume_cref(s.sub2.sub2.value1);
    consume_cref(s.sub2.sub2.value2);
    consume_cref(s.sub2.value3);
    consume_cref(s.value3);
}

template void destruct<string_very_complex1_struct>(void*);
template void copy<string_very_complex1_struct>(void*, void*);
template void move<string_very_complex1_struct>(void*, void*);

// string_very_complex2_struct

template <>
void construct<string_very_complex2_struct>(void* obj)
{
    string_very_complex2_struct& s =
        *static_cast<string_very_complex2_struct*>(new (obj) string_very_complex2_struct);

    s.value1 = 0x1111;
    s.sub2.value1 = "abcdefghijklmnopqrstuvwxyz";
    s.sub2.sub2.value1 = 0x2222;
    s.sub2.sub2.value2 = 0x3333;
    s.sub2.value3 = "abcdefghijklmnopqrstuvwxyz";
    s.value3 = 0x4444;
}

template <>
void dissect<string_very_complex2_struct>(void* obj)
{
    string_very_complex2_struct& s = *static_cast<string_very_complex2_struct*>(obj);

    consume_cref(s.value1);
    consume_cref(s.sub2.value1);
    consume_cref(s.sub2.sub2.value1);
    consume_cref(s.sub2.sub2.value2);
    consume_cref(s.sub2.value3);
    consume_cref(s.value3);
}

template void destruct<string_very_complex2_struct>(void*);
template void copy<string_very_complex2_struct>(void*, void*);
template void move<string_very_complex2_struct>(void*, void*);
