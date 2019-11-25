#pragma once

#include "../../ext/byte_array.h"

namespace smart_block {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"

// ArraySimple1

static const char* ArraySimple1Str = "{ [ 0x1111, 0x2222, 0x3333 ] }";

static const auto ArraySimple1Data = byte_array({
    0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x33, 0x33, 0x00, 0x00 });

// ArraySimple2

static const char* ArraySimple2Str = "{ [ int_struct(0x1111), int_struct(0x2222), int_struct(0x3333) ] }";

static const auto ArraySimple2Data = byte_array({
    0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x33, 0x33, 0x00, 0x00 });

// ArraySimple3

static const char* ArraySimple3Str = "{ [ 'abcdefghi', 'jklmnopqr', 'stuvwxyz' ] }";

static const auto ArraySimple3Data = byte_array({
      32,   10,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',
    '\0',   10,  'j',  'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r',
    '\0',    9,  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });

// ArrayBlock1

static const char* ArrayBlock1Str = "{ [ { 0x1111, 0x2222 }, { 0x3333, 0x4444 }, { 0x5555, 0x6666 } ] }";

static const auto ArrayBlock1Data = byte_array({
    0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x33, 0x33, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
    0x55, 0x55, 0x00, 0x00, 0x66, 0x66, 0x00, 0x00 });

// ArrayBlock2

static const char* ArrayBlock2Str = "{ [ { 'abcde', 'fghij' }, { 'klmn', 'opqr' }, { 'stuv', 'wxyz' } ] }";

static const auto ArrayBlock2Data = byte_array({
     41,   14,    6,    6,  'a',  'b',  'c',  'd',  'e', '\0',  'f',  'g',  'h',  'i',  'j', '\0',
     12,    5,    5,  'k',  'l',  'm',  'n', '\0',  'o',  'p',  'q',  'r', '\0',   12,    5,    5,
    's',  't',  'u',  'v', '\0',  'w',  'x',  'y',  'z', '\0' });

// ArrayBlock3

static const char* ArrayBlock3Str =
    "{ [ { 0x1111, 'abcdefghi' }, { 0x2222, 'jklmnopqr' }, { 0x3333, 'stuvwxyz' } ] }";

static const auto ArrayBlock3Data = byte_array({
      47,   15,   10, 0x11, 0x11, 0x00, 0x00,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',
    '\0',   15,   10, 0x22, 0x22, 0x00, 0x00,  'j',  'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r',
    '\0',   14,    9, 0x33, 0x33, 0x00, 0x00,  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });

// ArrayBlock4

static const char* ArrayBlock4Str =
    "{ [ { 'abcdefghi', 0x1111 }, { 'jklmnopqr', 0x2222 }, { 'stuvwxyz', 0x3333 } ] }";

static const auto ArrayBlock4Data = byte_array({
      47,   15,   10,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i', '\0', 0x11, 0x11, 0x00,
    0x00,   15,   10,  'j',  'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r', '\0', 0x22, 0x22, 0x00,
    0x00,   14,    9,  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0', 0x33, 0x33, 0x00, 0x00 });

#pragma clang diagnostic pop

} // smart_block::
