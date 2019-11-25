#pragma once

#include "facade.h"

#include <array>
#include <memory>
#include <cstring>

namespace smart_block {

namespace ts = type_sequence;

// block_storage implementations

namespace internal {

template <bool Plain, typename... Ts> class block_storage_impl;

// plain

template <typename... Ts>
class block_storage_impl<true, Ts...>
{
    using types = typename facade_traits<block_cfacade<Ts...>>::types;

    std::array<byte, plainable_block_length(types())> _data = { };

public:
    block_storage_impl() = default;

    block_storage_impl(size_t, const byte* data)
    { memcpy(_data.data(), data, _data.size()); }

    block_storage_impl(byte* data)
    { memmove(_data.data(), data, _data.size()); }

    byte* data() { return _data.data(); }

    const byte* data() const { return _data.data(); }

    byte* release_data() { return _data.data(); }

    friend void swap(block_storage_impl& lhs, block_storage_impl& rhs)
    { swap(lhs._data, rhs._data); }
};

// not plain

template <typename... Ts>
class block_storage_impl<false, Ts...>
{
    byte* _data;

public:
    block_storage_impl(size_t length) : _data(new byte[length])
    { memset(_data, 0, length); }

    block_storage_impl(size_t length, const byte* data) : _data(new byte[length])
    { memcpy(_data, data, length); }

    block_storage_impl(byte* data) : _data(data) { }

    block_storage_impl(block_storage_impl&& other) : _data(nullptr)
    { swap(_data, other._data); }

    ~block_storage_impl() { delete [] _data; }

    byte* data() { return _data; }

    const byte* data() const { return _data; }

    byte* release_data() {
        byte* result = _data;
        _data = nullptr;
        return result;
    }

    friend void swap(block_storage_impl& lhs, block_storage_impl& rhs)
    { swap(lhs._data, rhs._data); }
};

} // smart_block::detail

// block_storage

template <typename... Ts>
class block_storage : public internal::block_storage_impl<isAllPlainable<Ts...>, Ts...>
{
    static constexpr bool is_plain = isAllPlainable<Ts...>;

    using impl = internal::block_storage_impl<is_plain, Ts...>;

public:
    template <typename U = void, typename = std::enable_if_t<is_plain, U>>
    block_storage() { }

    block_storage(size_t length) : impl(length) { }

    block_storage(size_t length, const byte* data) : impl(length, data) { }

    block_storage(byte* data) : impl(data) { }

    byte* release_data() { return impl::release_data(); }
};

// block

template <typename... Ts>
class block : private block_storage<Ts...>, public block_facade<Ts...>
{
    static constexpr bool is_plain = isAllPlainable<Ts...>;

    using storage = block_storage<Ts...>;
    using facade  = block_facade<Ts...>;

public:
    using builder = builder<Ts...>;

    template <typename U = void, typename = std::enable_if_t<is_plain, U>>
    block() : facade(storage::data()) { }

    template <typename U = void, typename = std::enable_if_t<!is_plain, U>>
    block(size_t length) : storage(length), facade(storage::data()) { }

    block(const block& other)
      : storage(other.length(), other.data()), facade(storage::data())
    { }

    block(block&& other)
      : storage(other.release_data()), facade(storage::data())
    { }

    block& operator =(const block& other)
    {
        block::~block();
        new (this) block(other);

        return *this;
    }

    block& operator =(block&& other)
    {
        block::~block();
        new (this) block(std::move(other));

        return *this;
    }

    template <typename U = void, typename = std::enable_if_t<!is_plain, U>>
    block(storage&& other)
      : storage(other.release_data()), facade(storage::data())
    { }

    byte* data() { return storage::data(); }

    const byte* data() const { return storage::data(); }

    size_t length() const { return facade::length(); }

    std::span<byte> span() { return { data(), length() }; }

    byte* release_data() { return storage::release_data(); }

    friend void swap(block& lhs, block& rhs)
    {
        swap(static_cast<storage&>(lhs), static_cast<storage&>(rhs));

        if (!is_plain)
            swap(static_cast<facade&>(lhs), static_cast<facade&>(rhs));
    }
};

template <typename... Ts>
struct facade_traits<block<Ts...>> : facade_traits<block_cfacade<Ts...>> { };

template <typename... Ts>
ostream& operator <<(ostream& os, const block<Ts...>& block)
{
    os << static_cast<const block_facade<Ts...>&>(block);

    return os;
}

// transform_item

template <typename... Ts>
struct transform_item<block<Ts...>, std::enable_if_t<isAllPlainable<Ts...>>>
{
    static constexpr bool is_plain = true;

    using value_type = block_cfacade<Ts...>;
    using builder    = builder<Ts...>;
    using editor     = block_facade<Ts...>;

    static editor get_editor(builder& builder) { return { builder.data() }; }

    static void pack(const builder& builder, byte* target)
    { memcpy(target, &builder, plainable_block_length(type_sequence<Ts...>())); }

    static value_type unpack(const byte* target, size_t) { return { target }; }
};

template <typename... Ts>
struct transform_item<block<Ts...>, std::enable_if_t<!isAllPlainable<Ts...>>>
{
    static constexpr bool is_plain = false;

    using value_type = block_cfacade<Ts...>;
    using builder    = builder<Ts...>;
    using editor     = builder&;

    static editor get_editor(builder& builder) { return builder; }

    static constexpr size_t length(const builder& builder) { return builder.block_length(); }

    static void pack(const builder& builder, byte* target) { builder.emplace(target); }

    static value_type unpack(const byte* target, size_t) { return { target }; }
};

} // smart_block::

#define BUILDABLE_BLOCK_METHODS                                          \
const base& as_base() const { return static_cast<const base&>(*this); }  \
                                                                         \
byte* data() { return storage::data(); }                                 \
                                                                         \
const byte* data() const { return storage::data(); }                     \
                                                                         \
size_t length() const { return base::length(); }                         \
                                                                         \
std::span<byte> span() { return { data(), length() }; }                  \
                                                                         \
self& operator =(const self& other) {                                    \
    self::~self(); new (this) self(other);                               \
    return *this;                                                        \
}                                                                        \
                                                                         \
self& operator =(self&& other) {                                         \
    self::~self(); new (this) self(std::move(other));                    \
    return *this;                                                        \
}                                                                        \
                                                                         \
friend bool operator ==(const self& lhs, const self& rhs) {              \
    return lhs.as_base() == rhs.as_base();                               \
}                                                                        \
                                                                         \
friend bool operator ==(const self& lhs, const cfacade& rhs) {           \
    return lhs.as_base() == rhs;                                         \
}                                                                        \
                                                                         \
friend std::ostream& operator <<(std::ostream& os, const self& val) {    \
    os << static_cast<const base&>(val);                                 \
    return os;                                                           \
}

#define CONSTRUCTABLE_BLOCK_METHODS                            \
BUILDABLE_BLOCK_METHODS                                        \
                                                               \
friend bool operator ==(const self& lhs, const facade& rhs) {  \
    return static_cast<const base&>(lhs) == rhs;               \
}
