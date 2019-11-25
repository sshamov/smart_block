#pragma once

#include "../block.h"
#include "../builder.h"
#include "../std/string.h"

using smart_block::block;
using std::string;

// blocks

using basic_simple_block = block<uint32_t, uint32_t>;
using basic_complex_block = block<uint32_t, basic_simple_block, uint32_t>;
using basic_very_complex_block = block<uint32_t, basic_complex_block, uint32_t>;
using string_simple1_block = block<string, uint32_t, string>;
using string_simple2_block = block<uint32_t, string, uint32_t>;
using string_complex1_block = block<string, block<uint32_t, uint32_t>, string>;
using string_complex2_block = block<uint32_t, block<string, string>, uint32_t>;
using string_very_complex1_block = block<string, string_complex2_block, string>;
using string_very_complex2_block = block<uint32_t, string_complex1_block, uint32_t>;

// structs

struct basic_simple_struct
{
    uint32_t value1;
    uint32_t value2;
};

struct basic_complex_struct
{
    uint32_t            value1;
    basic_simple_struct sub2;
    uint32_t            value3;
};

struct basic_very_complex_struct
{
    uint32_t             value1;
    basic_complex_struct sub2;
    uint32_t             value3;
};

struct string_simple1_struct
{
    string   value1;
    uint32_t value2;
    string   value3;
};

struct string_simple2_struct
{
    uint32_t value1;
    string   value2;
    uint32_t value3;
};

struct string_complex1_struct
{
    string              value1;
    basic_simple_struct sub2;
    string              value3;
};

struct string_complex2_struct
{
    uint32_t           value1;
    struct {
        string value1;
        string value2;
    }                  sub2;
    uint32_t           value3;
};

struct string_very_complex1_struct
{
    string                 value1;
    string_complex2_struct sub2;
    string                 value3;
};

struct string_very_complex2_struct
{
    uint32_t               value1;
    string_complex1_struct sub2;
    uint32_t               value3;
};

// actions

template <typename T> void construct(void* obj);
template <typename T> void destruct(void* obj);
template <typename T> void dissect(void* obj);
template <typename T> void copy(void* src, void* dst);
template <typename T> void move(void* src, void* dst);

namespace noopt {

template <typename T> void construct(void* obj);
template <typename T> void destruct(void* obj);
template <typename T> void dissect(void* obj);
template <typename T> void copy(void* src, void* dst);
template <typename T> void move(void* src, void* dst);

} //noopt::
