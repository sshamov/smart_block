#pragma once

#include "../utils.h"

#include <optional>
#include <ostream>
#include <cstddef>

namespace smart_block {

using std::byte;
using std::optional;

// transform_item

template <typename T>
struct transform_item<optional<T>>
{
    static constexpr bool is_plain = false;

    using value_type = optional<value_of<T>>;
    using builder    = optional<builder_of<T>>;
    using editor     = editor_of<T>;

    static editor get_editor(builder& builder)
    {
        if (!builder.has_value()) builder.emplace();

        return smart_block::get_editor<T>(builder.value());
    }

    static size_t length(const builder& builder)
    { return builder.has_value() ? value_length<T>(builder.value()) : 0; }

    static void pack(const builder& builder, byte* target)
    { if (builder.has_value()) pack_value<T>(builder.value(), target); }

    static value_type unpack(const byte* target, size_t length)
    { return length > 0 ? value_type(unpack_value<T>(target, length)) : std::nullopt; }

    static void print(ostream& os, const byte* target, size_t length)
    {
        if (length > 0)
            print_value<T>(os, target, length);
        else
            os << "<null>";
    }
};

} // smart_block::
