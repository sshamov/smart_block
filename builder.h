#pragma once

#include "facade.h"

namespace smart_block {

// builder

namespace internal {

template <bool Plain, typename... Ts> class builder_impl;

template <typename... Ts>
class builder_impl<true, Ts...> : public block_storage<Ts...>
{
};

template <typename... Ts>
class builder_impl<false, Ts...>
{
    using types = type_sequence<Ts...>;

    template <size_t Index>
    using type = ts::get_t<Index, types>;

    template <size_t Index>
    using item_builder = builder_of<type<Index>>;

    template <size_t Index>
    using item_editor = editor_of<type<Index>>;

    template <size_t Index>
    static constexpr size_t _offset = builder_length(ts::trunc_t<Index, types>());

    std::array<byte, builder_length(types())> _data = { };

public:
    builder_impl()
    {
        ts::for_each<types>(
            [&](auto* p, auto i) {
                using item_t = std::decay_t<decltype(*p)>;

                if constexpr (eval<item_t>(hasTransform))
                    new (&_data[_offset<i.value>]) typename transform_item<item_t>::builder();
                else
                    new (&_data[_offset<i.value>]) item_t();
            });
    }

    builder_impl(builder_impl&& other) : builder_impl() { swap(*this, other); }

    ~builder_impl()
    {
        ts::for_each<types>(
            [&](auto*, auto i) {
                using item_t = std::decay_t<decltype(item<i.value>())>;
                item<i.value>().~item_t();
            });
    }

    static constexpr size_t size() { return ts::size<types>; }

    constexpr size_t block_length() const
    {
        size_t block_size =
            ts::count_v<type_sequence<Ts...>, IsNotPlain> +
            ts::for_each<types, meta::add_assign<size_t>>(
                [&](auto* p, auto i) {
                    using item_t = std::decay_t<decltype(*p)>;
                    return value_length<item_t>(item<i.value>());
                });

        return block_size;
    }

    void set(builder_impl&& val) { swap(*this, val); }

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

    template <size_t Index, typename U, typename = BConstraint<item_editor<Index>, U, CanBeSetFromRv>>
    void set(U&& val)
    {
        using editor = item_editor<Index>;

        editor e = edit<Index>();

        if constexpr (eval<editor, U>(hasSetFromRv))
            e.set(std::move(val));
        else if constexpr (eval<editor, U>(isAssignableFromRv))
            e = std::move(val);
    }

    template <size_t Index, typename = Constraint<std::decay_t<item_editor<Index>>, IsBlockEditor>>
    item_editor<Index> sub() { return edit<Index>(); }

    template <size_t Index>
    item_editor<Index> edit() { return get_editor<type<Index>>(item<Index>()); }

    template <size_t Index>
    item_builder<Index>& item()
    { return reinterpret_cast<item_builder<Index>&>(_data[_offset<Index>]); }

    template <size_t Index>
    const item_builder<Index>& item() const
    { return reinterpret_cast<const item_builder<Index>&>(_data[_offset<Index>]); }

    void emplace(byte* target) const
    {
        byte* header = target;
        byte* item_target = header + ts::count_v<types, IsNotPlain>;

        ts::for_each<types>(
            [&](auto*, auto i) {
                using item_t = type<i.value>;

                pack_value<item_t>(item<i.value>(), item_target);

                size_t item_length = value_length<item_t>(item<i.value>());

                if constexpr (eval<item_t>(isNotPlain))
                    *header++ = static_cast<byte>(item_length);

                item_target += item_length;
            });
    }

    block<Ts...> build() const
    {
        block<Ts...> result{block_length()};

        emplace(result.data());

        return result;
    }

    block_storage<Ts...> build_storage() const
    {
        block_storage<Ts...> result(block_length());

        emplace(result.data());

        return result;
    }

    friend void swap(builder_impl& lhs, builder_impl& rhs)
    {
        using std::swap;

        ts::for_each<types>(
            [&](auto*, auto i) { swap(lhs.item<i.value>(), rhs.item<i.value>()); }
        );
    }
};

} // smart_block::internal::

template <typename... Ts>
class builder : public internal::builder_impl<isAllPlainable<Ts...>, Ts...>
{
};

} // smart_block::

#define BUILDER_METHODS                                \
block build() const { return base::build_storage(); }  \
                                                       \
base& as_base() { return static_cast<base&>(*this); }
