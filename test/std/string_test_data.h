#pragma once

#include "../../ext/byte_array.h"

namespace smart_block {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"

// StringSimple1

static const char* StringSimple1Str  = "{ 'abcdefghijklm', 0x1111, 'nopqrstuvwxyz' }";
static const auto  StringSimple1Data = byte_array({
      14,   14,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm', '\0',
    0x11, 0x11, 0x00, 0x00,  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',
     'z', '\0' });

// StringSimple2

static const char* StringSimple2Str  = "{ 0x1111, 'abcdefghijklmnopqrstuvwxyz', 0x2222 }";
static const auto  StringSimple2Data = byte_array({
      27, 0x11, 0x11, 0x00, 0x00,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',
     'l',  'm',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0',
    0x22, 0x22, 0x00, 0x00 });

// StringComplex1

static const char* StringComplex1Str  = "{ 'abcdefghijklm', { 0x1111, 0x2222 }, 'nopqrstuvwxyz' }";
static const char* StringComplex1Str2 = "{ 'abcdefghijklm',\n"
                                        "  { 0x1111, 0x2222 },\n"
                                        "  'nopqrstuvwxyz' }";
static const auto  StringComplex1Data = byte_array({
      14,   14,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm', '\0',
    0x11, 0x11, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00,  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',
     'v',  'w',  'x',  'y',  'z', '\0' });

// StringComplex2

static const char* StringComplex2Str  = "{ 0x1111, { 'abcdefghijklm', 'nopqrstuvwxyz' }, 0x2222 }";
static const char* StringComplex2Str2 = "{ 0x1111,\n"
                                        "  { 'abcdefghijklm', 'nopqrstuvwxyz' },\n"
                                        "  0x2222 }";
static const auto  StringComplex2Data = byte_array({
     30, 0x11, 0x11, 0x00, 0x00,   14,   14,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',
    'k',  'l',  'm', '\0',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',
    'y',  'z', '\0', 0x22, 0x22, 0x00, 0x00 });

// StringVeryComplex1

static const char* StringVeryComplex1Str  = "{ 'abcdefg', { 0x1111, { 'hijklm'"
                                              ", 'nopqrs' }, 0x2222 }, 'tuvwxyz' }";
static const char* StringVeryComplex1Str2 = "{ 'abcdefg',\n"
                                            "  { 0x1111,\n"
                                            "    { 'hijklm', 'nopqrs' },\n"
                                            "    0x2222 },\n"
                                            "  'tuvwxyz' }";
static const auto  StringVeryComplex1Data = byte_array({
       8,   25,    8,  'a',  'b',  'c',  'd',  'e',  'f',  'g', '\0',   16, 0x11, 0x11, 0x00, 0x00,    7,
       7,  'h',  'i',  'j',  'k',  'l',  'm', '\0',  'n',  'o',  'p',  'q',  'r',  's', '\0', 0x22, 0x22,
    0x00, 0x00,  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0' });

// StringVeryComplex2

static const char* StringVeryComplex2Str  = "{ 0x1111, { 'abcdefghijklm', { 0x2222"
                                              ", 0x3333 }, 'nopqrstuvwxyz' }, 0x4444 }";
static const char* StringVeryComplex2Str2 = "{ 0x1111,\n"
                                            "  { 'abcdefghijklm',\n"
                                            "    { 0x2222, 0x3333 },\n"
                                            "    'nopqrstuvwxyz' },\n"
                                            "  0x4444 }";
static const auto  StringVeryComplex2Data = byte_array({
     38, 0x11, 0x11, 0x00, 0x00,   14,   14,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',
    'k',  'l',  'm', '\0', 0x22, 0x22, 0x00, 0x00, 0x33, 0x33, 0x00, 0x00,  'n',  'o',  'p',  'q',  'r',
    's',  't',  'u',  'v',  'w',  'x',  'y',  'z', '\0', 0x44, 0x44, 0x00, 0x00 });

#pragma clang diagnostic push

} // smart_block::
