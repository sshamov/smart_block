#pragma once

#include "ext/span.h"
#include "utils.h"

#include <ostream>
#include <iomanip>

namespace smart_block {

using std::byte;

namespace ts = type_sequence;

namespace internal {

// block_(c)facade implementations

template <bool Plain, typename... Ts> class block_cfacade_impl;

template <bool Plain, typename... Ts> class block_facade_impl;

// plain

template <typename... Ts>
class block_cfacade_impl<true, Ts...>
{
    using types = type_sequence<Ts...>;

    template <size_t Index>
    using type = ts::get_t<Index, types>;

    template <size_t Index>
    using item_builder = builder_of<type<Index>>;

    template <size_t Index>
    using item_unpacked = type_if_or<type<Index>, HasTransform, value_of<type<Index>>, const type<Index>&>;

    template <size_t Index>
    static constexpr size_t _offset = plainable_block_length(ts::trunc_t<Index, types>());

    const byte* _block_data = nullptr;

public:
    block_cfacade_impl(const byte* data) : _block_data(data) { }

    static constexpr size_t size() { return ts::size<types>; }

    const byte* data() const { return _block_data; }

    static constexpr size_t length() { return plainable_block_length(types()); }

    std::span<const byte> span() const { return { data(), length() }; }

    template <size_t Index, typename = Constraint<item_unpacked<Index>, IsBlockFacade>>
    item_unpacked<Index> sub() const { return get<Index>(); }

    template <size_t Index>
    item_unpacked<Index> get() const
    { return unpack_value<type<Index>>(_block_data + _offset<Index>, sizeof(item_builder<Index>)); }

    template <size_t Index>
    void print(ostream& os) const
    { print_value<type<Index>>(os, _block_data + _offset<Index>, sizeof(item_builder<Index>)); }

    friend void swap(block_cfacade_impl& lhs, block_cfacade_impl& rhs)
    { swap(lhs._block_data, rhs._block_data); }

    friend bool operator ==(const block_cfacade_impl& lhs, const block_cfacade_impl& rhs) {
        return lhs.length() == rhs.length() &&
               memcmp(lhs._block_data, rhs._block_data, lhs.length()) == 0;
    }

    friend facade_traits<block_cfacade<Ts...>>;
};

template <typename... Ts>
class block_facade_impl<true, Ts...> : public internal::block_cfacade_impl<true, Ts...>
{
    using cfacade_impl = internal::block_cfacade_impl<true, Ts...>;

    using types = type_sequence<Ts...>;

    template <size_t Index>
    using type = ts::get_t<Index, types>;

    template <size_t Index>
    using item_builder = builder_of<type<Index>>;

    template <size_t Index>
    using item_editor = editor_of<type<Index>>;

    template <size_t Index>
    static constexpr size_t _offset = plainable_block_length(ts::trunc_t<Index, types>());

    byte* deconst_block_data()
    { return const_cast<byte*>(cfacade_impl::data()); }

public:
    block_facade_impl(byte* data) : cfacade_impl(data) { }

    using cfacade_impl::data;
    using cfacade_impl::sub;

    byte* data() { return deconst_block_data(); }

    void set(const block_storage<Ts...>& val)
    { memcpy(deconst_block_data(), &val, plainable_block_length(types())); }

    template <size_t Index, typename = Constraint<type<Index>, HasTransform>>
    void set(const item_builder<Index>& val)
    {
        using builder = item_builder<Index>;

        builder& stored = *reinterpret_cast<builder*>(deconst_block_data() + _offset<Index>);

        memcpy(&stored, &val, sizeof(builder));
    }

    template <size_t Index, typename U, typename = BConstraint<item_editor<Index>, U, CanBeSetFromCref>>
    void set(const U& val)
    {
        using editor = item_editor<Index>;

        editor e = edit<Index>();

        if constexpr (eval<editor, U>(hasSetFromCref))
            e.set(val);
        else if constexpr (eval<editor, U>(isAssignableFromCref))
            e = val;
        else if constexpr (eval<editor, U>(isAggregatableFromCref))
            e = { val };
    }

    template <size_t Index, typename = Constraint<std::decay_t<item_editor<Index>>, IsBlockFacade>>
    item_editor<Index> sub() { return edit<Index>(); }

    template <size_t Index>
    item_editor<Index> edit()
    {
        using builder = item_builder<Index>;

        builder& stored = *reinterpret_cast<builder*>(deconst_block_data() + _offset<Index>);

        return get_editor<type<Index>>(stored);
    }
};

// not plain

template <typename... Ts>
class block_cfacade_impl<false, Ts...>
{
    using types = type_sequence<Ts...>;

    template <size_t Index>
    using type = ts::get_t<Index, types>;

    template <size_t Index>
    using item_unpacked = type_if_or<type<Index>, HasTransform, value_of<type<Index>>, const type<Index>&>;

    const byte* _block_data = nullptr;

    template <size_t Index>
    size_t offset() const
    {
        const byte* header = _block_data;
        size_t      result = ts::count_v<types, IsNotPlain>;

        ts::for_each<ts::trunc_t<Index, types>>(
            [&](auto* p, auto) {
                using item_t = std::decay_t<decltype(*p)>;

                if constexpr (eval<item_t>(isNotPlain))
                    result += static_cast<size_t>(*header++);
                else
                    result += sizeof(builder_of<item_t>);
            });

        return result;
    }

    template <size_t Index>
    size_t length() const
    {
        constexpr size_t header_offset = ts::count_v<ts::trunc_t<Index, types>, IsNotPlain>;

        return static_cast<size_t>(_block_data[header_offset]);
    }

public:
    block_cfacade_impl(const byte* data) : _block_data(data) { }

    block_cfacade_impl(const block_cfacade_impl& other) : _block_data(other._block_data) { }

    block_cfacade_impl(block_cfacade_impl&& other) { swap(_block_data, other._block_data); }

    static constexpr size_t size() { return ts::size<types>; }

    const byte* data() const { return _block_data; }

    size_t length() const { return offset<size()>(); }

    std::span<const byte> span() const { return { data(), length() }; }

    template <size_t Index, typename = Constraint<item_unpacked<Index>, IsBlockFacade>>
    item_unpacked<Index> sub() const { return get<Index>(); }

    template <size_t Index>
    item_unpacked<Index> get() const
    {
        size_t item_length = eval<type<Index>>(isNotPlain)
            ? length<Index>()
            : sizeof(builder_of<type<Index>>);

        return unpack_value<type<Index>>(_block_data + offset<Index>(), item_length);
    }

    template <size_t Index, typename = Constraint<type<Index>, HasRawData>>
    std::span<const byte> raw_data() const
    { return transform_item<type<Index>>::raw_data(_block_data + offset<Index>(), length<Index>()); }

    template <size_t Index>
    void print(ostream& os) const
    {
        size_t item_length = eval<type<Index>>(isNotPlain)
            ? length<Index>()
            : sizeof(builder_of<type<Index>>);

        return print_value<type<Index>>(os, _block_data + offset<Index>(), item_length);
    }

    friend void swap(block_cfacade_impl& lhs, block_cfacade_impl& rhs)
    { swap(lhs._block_data, rhs._block_data); }

    friend bool operator ==(const block_cfacade_impl& lhs, const block_cfacade_impl& rhs) {
        return lhs.length() == rhs.length() &&
               memcmp(lhs._block_data, rhs._block_data, lhs.length()) == 0;
    }

    friend facade_traits<block_cfacade<Ts...>>;
};

template <typename... Ts>
class block_facade_impl<false, Ts...> : public internal::block_cfacade_impl<false, Ts...>
{
    using cfacade_impl = internal::block_cfacade_impl<false, Ts...>;

public:
    block_facade_impl(byte* data) : cfacade_impl(data) { }
};

} // smart_block::internal::

// block_(c)facade

template <typename... Ts>
class block_cfacade : public internal::block_cfacade_impl<isAllPlainable<Ts...>, Ts...>
{
    using impl = internal::block_cfacade_impl<isAllPlainable<Ts...>, Ts...>;

public:
    block_cfacade(const byte* data) : impl(data) { }
};

template <typename... Ts>
class block_facade : public internal::block_facade_impl<isAllPlainable<Ts...>, Ts...>
{
    using impl = internal::block_facade_impl<isAllPlainable<Ts...>, Ts...>;

public:
    block_facade(byte* data) : impl(data) { }
};

// facade_traits

template <typename... Ts>
struct facade_traits<block_cfacade<Ts...>>
{
    using cfacade = block_cfacade<Ts...>;
    using facade  = block_facade<Ts...>;
    using types   = typename cfacade::types;

    static constexpr size_t size = cfacade::size();
};

template <typename... Ts>
struct facade_traits<block_facade<Ts...>> : facade_traits<block_cfacade<Ts...>> { };

// iomanip for vertical arrangement

class topdown
{
    short _min_complexity;

public:
    topdown(short min_complexity = 0) : _min_complexity(min_complexity) { }

    static int index() {
        static int val = std::ios_base::xalloc();
        return val;
    }

    friend std::ostream& operator <<(std::ostream& os, const topdown& manip) {
        os.iword(topdown::index()) = 2 + (manip._min_complexity << 16);
        return os;
    }
};

// operator <<

template <typename... Ts>
ostream& operator <<(ostream& os, const block_cfacade<Ts...>& block)
{
    using facade = block_facade<Ts...>;
    using types  = typename facade_traits<std::decay_t<decltype(block)>>::types;

    long   topdown_params = os.iword(topdown::index());
    bool   topdown = topdown_params > 0;
    size_t min_complexity = static_cast<unsigned short>(topdown_params >> 16);
    size_t indent = static_cast<unsigned short>(topdown_params & 0xffff);

    if (block_complexity(static_cast<facade*>(nullptr)) < min_complexity) {
        topdown = false;
        os.iword(topdown::index()) = 0;
    }

    os << "{ ";

    ts::for_each<types>(
        [&](auto* p, auto i) {
            using item_t = std::decay_t<decltype(*p)>;

            if (i.value > 0) os << (topdown ? ",\n" + std::string(indent, ' ') : ", ");

            if constexpr (eval<item_t>(isBlock)) {
                if (topdown) os.iword(topdown::index()) += 2;

                os << block.template sub<i.value>();

                if (topdown) os.iword(topdown::index()) -= 2;
            }
            else
                block.template print<i.value>(os);
        });

    os << " }";

    os.iword(topdown::index()) = indent > 2 ? topdown_params : 0;

    return os;
}

template <typename... Ts>
ostream& operator <<(ostream& os, const block_facade<Ts...>& block)
{
    os << block_cfacade<Ts...>(block.data());

    return os;
}

} // smart_block::

// STD_FACADE_METHODS

#define FACADE_METHODS                                                   \
const byte* data() const { return base::data(); }                        \
                                                                         \
size_t length() const { return base::length(); }                         \
                                                                         \
std::span<const byte> span() const { return base::span(); }              \
                                                                         \
base& as_base() { return static_cast<base&>(*this); }                    \
                                                                         \
const base& as_base() const { return static_cast<const base&>(*this); }  \
                                                                         \
friend std::ostream& operator <<(std::ostream& os, const self& val) {    \
    return os << val.as_base();                                          \
}
