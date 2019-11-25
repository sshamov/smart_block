#pragma once

#include "../ext/span.h"
#include "../utils.h"

#include <array>
#include <cstddef>
#include <ostream>

namespace smart_block {

using std::byte;
using std::array;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"

class index_out_of_range : public std::exception
{
    std::string _what;

public:
    index_out_of_range() : _what("index_out_of_range") { }

    index_out_of_range& wrong_index(size_t index, size_t max)
    {
        _what += ": requested index (" + std::to_string(index);
        _what += ") exceeds maximum (" + std::to_string(max) + ")";

        return *this;
    }

    const char* what() const noexcept { return _what.c_str(); }
};

#pragma clang diagnostic push

// array_cfacade

template <typename T, size_t N, typename = void> class array_cfacade;

template <typename T, size_t N>
class array_cfacade<T, N, Constraint<T, IsPlain>>
{
    using unpacked_value = type_if_or<T, HasTransform, value_of<T>, const T&>;

    const byte* _data;

    constexpr size_t item_length() const { return sizeof(builder_of<T>); }

    size_t item_offset(size_t index) const { return index * item_length(); }

public:
    array_cfacade(const byte* data) : _data(data) { }

    unpacked_value operator [](size_t index) const
    {
        if (index > N-1) throw index_out_of_range().wrong_index(index, N-1);

        return unpack_value<T>(_data + item_offset(index), item_length());
    }
};

template <typename T, size_t N>
class array_cfacade<T, N, Constraint<T, IsNotPlain>>
{
    const byte* _data;

    size_t item_offset(size_t index) const
    {
        size_t result = 0;

        while (index-- > 0) result += 1 + static_cast<size_t>(_data[result]);

        return result;
    }

public:
    array_cfacade(const byte* data) : _data(data) { }

    value_of<T> operator [](size_t index) const
    {
        if (index > N-1) throw index_out_of_range().wrong_index(index, N-1);

        size_t offset = item_offset(index);
        size_t length = static_cast<size_t>(_data[offset++]);

        return transform_item<T>::unpack(_data + offset, length);
    }

    template <typename U = T, typename = Constraint<U, HasRawData>>
    std::span<const byte> raw_data(size_t index) const
    {
        if (index > N-1) throw index_out_of_range().wrong_index(index, N-1);

        size_t offset = item_offset(index);
        size_t length = static_cast<size_t>(_data[offset++]);

        return transform_item<T>::raw_data(_data + offset, length);
    }
};

// array_facade

template <typename T, size_t N>
class array_facade
{
    builder_of<T>* _items;

public:
    array_facade(builder_of<T>* items) : _items(items) { }

    editor_of<T> operator [](size_t index)
    {
        if (index > N-1) throw index_out_of_range().wrong_index(index, N-1);

        return get_editor<T>(*(_items + index));
    }
};

// transform_item

template <typename T, size_t N>
struct transform_item<array<T, N>>
{
    static constexpr bool is_plain = eval<T>(isPlain);

    using value_type = array_cfacade<T, N>;
    using builder    = array<builder_of<T>, N>;
    using editor     = array_facade<T, N>;

    static editor get_editor(builder& builder) { return { builder.data() }; }

    template <typename U = T, typename = Constraint<U, IsNotPlain>>
    static size_t length(const builder& builder)
    {
        size_t len = N;

        for (size_t i = 0; i < N; ++i)
            len += transform_item<T>::length(builder[i]);

        return len;
    }

    template <typename U = T, typename = Constraint<U, IsNotPlain>>
    static void pack(const builder& builder, byte* target)
    {
        for (size_t i = 0; i < N; ++i) {
            size_t item_length = transform_item<T>::length(builder[i]);

            *target++ = static_cast<byte>(item_length);
            transform_item<T>::pack(builder[i], target);

            target += item_length;
        }
    }

    static value_type unpack(const byte* target, size_t) { return { target }; }

    static void print(ostream& os, const byte* target, size_t)
    {
        os << "[ ";
        for (size_t i = 0; i < N; ++i) {
            if (i > 0) os << ", ";

            size_t item_length = eval<T>(isNotPlain)
                ? static_cast<size_t>(*target++)
                : sizeof(builder_of<T>);

            print_value<T>(os, target, item_length);

            target += item_length;
        }
        os << " ]";
    }
};

} // smart_block::
