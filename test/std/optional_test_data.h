#pragma once

#include "../../ext/byte_array.h"

namespace smart_block {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"

// OptionalSimple1

static const char* OptionalSimple1Str  = "{ 0x0, <null>, 0x0 }";
static const char* OptionalSimple1Str2 = "{ 0x1111, 0x2222, 0x3333 }";

static const auto OptionalSimple1Data  = byte_array({
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });

static const auto OptionalSimple1Data2 = byte_array({
    4, 0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x33, 0x33, 0x00, 0x00 });

// OptionalSimple2

static const char* OptionalSimple2Str  = "{ '', <null>, '' }";
static const char* OptionalSimple2Str2 = "{ 'abcdefghijklm', 0x1111, 'nopqrstuvwxyz' }";

static const auto OptionalSimple2Data  = byte_array({ 0x01, 0x00, 0x01, 0x00, 0x00 });

static const auto OptionalSimple2Data2 = byte_array({
      14,    4,   14,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',
    '\0', 0x11, 0x11, 0x00, 0x00,  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',
     'y',  'z', '\0' });

// OptionalSimple3

static const char* OptionalSimple3Str  = "{ 0x0, <null>, 0x0 }";
static const char* OptionalSimple3Str2 = "{ 0x1111, 'abcdefghijklmnopqrstuvwxyz', 0x2222 }";

static const auto OptionalSimple3Data  = byte_array({
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });

static const auto OptionalSimple3Data2 = byte_array({
      27, 0x11, 0x11, 0x00, 0x00,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',
     'l',  'm',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0',
    0x22, 0x22, 0x00, 0x00 });

// OptionalComplex1

static const char* OptionalComplex1Str  = "{ 0x0, <null>, 0x0 }";
static const char* OptionalComplex1Str2 = "{ 0x1111, { 0x2222, 0x3333 }, 0x4444 }";

static const auto OptionalComplex1Data  = byte_array({
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });

static const auto OptionalComplex1Data2 = byte_array({
       8, 0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00 });

// OptionalComplex2

static const char* OptionalComplex2Str  = "{ '', <null>, '' }";
static const char* OptionalComplex2Str2 = "{ 'abcdefghijklm', { 0x1111, 0x2222 }, 'nopqrstuvwxyz' }";

static const auto OptionalComplex2Data  = byte_array({ 0x01, 0x00, 0x01, 0x00, 0x00 });

static const auto OptionalComplex2Data2 = byte_array({
      14,    8,   14,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',
    '\0', 0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00,  'n',  'o',  'p',  'q',  'r',  's',  't',
     'u',  'v',  'w',  'x',  'y',  'z', '\0' });

// OptionalComplex3

static const char* OptionalComplex3Str  = "{ 0x0, <null>, 0x0 }";
static const char* OptionalComplex3Str2 = "{ 0x1111, { 'abcdefghijklm', 'nopqrstuvwxyz' }, 0x2222 }";

static const auto OptionalComplex3Data  = byte_array({
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });

static const auto OptionalComplex3Data2 = byte_array({
     30, 0x11, 0x11, 0x00, 0x00,   14,   14,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',
    'j',  'k',  'l',  'm', '\0',  'n',  'o',  'p',  'q',  'r',  's',  't',
    'u',  'v',  'w',  'x',  'y',  'z', '\0', 0x22, 0x22, 0x00, 0x00 });

// OptionalStrange1

static const char* OptionalStrange1Str  = "{ <null> }";
static const char* OptionalStrange1Str2 = "{ 0x1111 }";

static const auto OptionalStrange1Data  = byte_array({ 0x00 });

static const auto OptionalStrange1Data2 = byte_array({ 4, 0x11, 0x11, 0x00, 0x00 });

// OptionalStrange2

static const char* OptionalStrange2Str  = "{ <null> }";
static const char* OptionalStrange2Str2 = "{ 0x1111 }";

static const auto OptionalStrange2Data  = byte_array({ 0x00 });

static const auto OptionalStrange2Data2 = byte_array({ 4, 0x11, 0x11, 0x00, 0x00 });

// OptionalStrange3

static const char* OptionalStrange3Str  = "{ <null> }";
static const char* OptionalStrange3Str2 = "{ 'abcdefghijklmnopqrstuvwxyz' }";

static const auto OptionalStrange3Data  = byte_array({ 0x00 });

static const auto OptionalStrange3Data2 = byte_array({
     27,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
    'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });

#pragma clang diagnostic push

} // smart_block::
