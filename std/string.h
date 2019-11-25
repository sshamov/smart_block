#pragma once

#include "../ext/span.h"
#include "../fwd.h"

#include <string>
#include <cstddef>
#include <cstring>
#include <ostream>

namespace smart_block {

using std::byte;
using std::string;
using std::span;

// transform_item

template <>
struct transform_item<string>
{
    static constexpr bool is_plain = false;

    using value_type = string;
    using builder    = string;
    using editor     = string&;

    static editor get_editor(builder& builder) { return builder; }

    static size_t length(const builder& v) { return v.size() + 1; }

    static void pack(const builder& v, byte* target)
    { strcpy(reinterpret_cast<char*>(target), v.c_str()); }

    static value_type unpack(const byte* target, size_t length)
    { return { reinterpret_cast<const char*>(target), length - 1 }; }

    static void print(std::ostream& os, const byte* target, size_t length)
    { os << '\'' << unpack(target, length) << '\''; }

    static span<const byte> raw_data(const byte* target, size_t length)
    { return { target, length }; }
};

} // smart_block::
