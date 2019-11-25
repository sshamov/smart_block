#pragma once

#include "../../ext/byte_array.h"

namespace smart_block {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"

// VariantSimple1

static const char* VariantSimple1Str  = "{ 0x1111 }";
static const char* VariantSimple1Str2 = "{ int_struct(0x1111) }";

static const auto VariantSimple1Data  = byte_array({ 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
static const auto VariantSimple1Data2 = byte_array({ 0x11, 0x11, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 });

// VariantSimple2

static const char* VariantSimple2Str  = "{ 0x1111 }";
static const char* VariantSimple2Str2 = "{ 0x2222 }";

static const auto VariantSimple2Data  = byte_array({ 5, 0, 0x11, 0x11, 0x00, 0x00 });
static const auto VariantSimple2Data2 = byte_array({ 9, 1, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });

// VariantSimple3

static const char* VariantSimple3Str  = "{ 0x1111 }";
static const char* VariantSimple3Str2 = "{ 'abcdefghijklmnopqrstuvwxyz' }";

static const auto VariantSimple3Data  = byte_array({ 5, 0, 0x11, 0x11, 0x00, 0x00 });
static const auto VariantSimple3Data2 = byte_array({
     28,    1,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',
    'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });

// VariantSimple4

static const char* VariantSimple4Str  = "{ 'abcdefghijklmnopqrstuvwxyz' }";
static const char* VariantSimple4Str2 = "{ 0x1111 }";

static const auto VariantSimple4Data = byte_array({
     28,    0,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',
    'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });
static const auto VariantSimple4Data2  = byte_array({ 5, 1, 0x11, 0x11, 0x00, 0x00 });

// VariantMonostate1

static const char* VariantMonostate1Str  = "{ <null> }";
static const char* VariantMonostate1Str2 = "{ 0x1111 }";
static const char* VariantMonostate1Str3 = "{ 0x2222 }";

static const auto VariantMonostate1Data  = byte_array({ 0 });
static const auto VariantMonostate1Data2 = byte_array({ 5, 1, 0x11, 0x11, 0x00, 0x00 });
static const auto VariantMonostate1Data3 = byte_array({ 9, 2, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });

// VariantMonostate2

static const char* VariantMonostate2Str  = "{ <null> }";
static const char* VariantMonostate2Str2 = "{ 0x1111 }";
static const char* VariantMonostate2Str3 = "{ 'abcdefghijklmnopqrstuvwxyz' }";

static const auto VariantMonostate2Data  = byte_array({ 0 });
static const auto VariantMonostate2Data2 = byte_array({ 5, 1, 0x11, 0x11, 0x00, 0x00 });
static const auto VariantMonostate2Data3 = byte_array({
     28,    2,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',
    'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });

// VariantMonostate3

static const char* VariantMonostate3Str  = "{ <null> }";
static const char* VariantMonostate3Str2 = "{ 'abcdefghijklmnopqrstuvwxyz' }";
static const char* VariantMonostate3Str3 = "{ 0x1111 }";

static const auto VariantMonostate3Data  = byte_array({ 0 });
static const auto VariantMonostate3Data2 = byte_array({
     28,    1,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',
    'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });
static const auto VariantMonostate3Data3 = byte_array({ 5, 2, 0x11, 0x11, 0x00, 0x00 });

// VariantBlock1

static const char* VariantBlock1Str  = "{ { 0x1111, 0x2222 } }";
static const char* VariantBlock1Str2 = "{ { 0x1111, 0x2222 } }";

static const auto VariantBlock1Data  = byte_array({
    0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0 });
static const auto VariantBlock1Data2 = byte_array({
    0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 1 });

// VariantBlock2

static const char* VariantBlock2Str  = "{ { 0x1111, 0x2222 } }";
static const char* VariantBlock2Str2 = "{ { 0x1111, 0x2222 } }";

static const auto VariantBlock2Data  = byte_array({
       9,    0, 0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00 });
static const auto VariantBlock2Data2 = byte_array({
      17,    1, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });

// VariantBlock3

static const char* VariantBlock3Str  = "{ { 0x1111, 0x2222 } }";
static const char* VariantBlock3Str2 = "{ { 'abcdefghijklm', 'nopqrstuvwxyz' } }";

static const auto VariantBlock3Data  = byte_array({
      9,    0, 0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00 });
static const auto VariantBlock3Data2 = byte_array({
     31,    1,   14,   14,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',
    'm', '\0',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });

// VariantBlock4

static const char* VariantBlock4Str  = "{ { 'abcdefghijklm', 'nopqrstuvwxyz' } }";
static const char* VariantBlock4Str2 = "{ { 0x1111, 0x2222 } }";

static const auto VariantBlock4Data  = byte_array({
     31,    0,   14,   14,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',
    'm', '\0',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });
static const auto VariantBlock4Data2 = byte_array({
      9,    1, 0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00 });

// VariantBlock5

static const char* VariantBlock5Str  = "{ { 0x1111, 'abcdefghijklmnopqrstuvwxyz' } }";
static const char* VariantBlock5Str2 = "{ { 'abcdefghijklmnopqrstuvwxyz', 0x1111 } }";

static const auto VariantBlock5Data  = byte_array({
     33,    0,   27, 0x11, 0x11, 0x00, 0x00,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',
    'j',  'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',
    'z', '\0' });
static const auto VariantBlock5Data2 = byte_array({
     33,    1,   27,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',
    'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0', 0x11, 0x11,
   0x00, 0x00 });

#pragma clang diagnostic push

} // smart_block::
