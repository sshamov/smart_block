#pragma once

#include "ext/meta.h"
#include "ext/type_sequence.h"
#include "fwd.h"

namespace smart_block {

namespace ts = type_sequence;

namespace internal2 {

template <typename T>
using get_value_type = typename transform_item<T>::value_type;

template <typename T>
using get_builder = typename transform_item<T>::builder;

template <typename T>
using get_editor = typename transform_item<T>::editor;

} // smart_block::internal2::

// value_of

template <typename T>
using value_of = std::conditional_t<
    meta::is_complete_v<transform_item<T>>,
    meta::get_optional_t<internal2::get_value_type, T>,
    T
>;

// builder_of

template <typename T>
using builder_of = std::conditional_t<
    meta::is_complete_v<transform_item<T>>,
    meta::get_optional_t<internal2::get_builder, T>,
    T
>;

// editor_of

template <typename T>
using editor_of = std::conditional_t<
    meta::is_complete_v<transform_item<T>>,
    meta::get_optional_t<internal2::get_editor, T>,
    T&
>;

// is_all_plainable

template <typename... Ts>
constexpr bool is_all_plainable(type_sequence<Ts...>)
{
    using seq = type_sequence<Ts...>;

    if constexpr (ts::size<seq> == 1)
    {
        using item_t = ts::get_t<0, seq>;

        if constexpr (meta::is_complete_v<transform_item<item_t>>)
            return transform_item<item_t>::is_plain;
        else
            return std::is_trivially_copyable_v<item_t>;
    }
    else
        return (is_all_plainable(type_sequence<Ts>()) && ...);
}

template <typename... Ts>
constexpr bool isAllPlainable = is_all_plainable(type_sequence<Ts...>());

// plainable_block_length

template <typename... Ts>
constexpr size_t plainable_block_length(type_sequence<Ts...>)
{
    return (0 + ... + sizeof(builder_of<Ts>));
}

// builder_length

template <typename... Ts>
constexpr size_t builder_length(type_sequence<Ts...>)
{
    return (0 + ... + sizeof(builder_of<Ts>));
}

// block_complexity

template <typename> struct is_block : std::false_type { };

template <typename... Ts> struct is_block<block<Ts...>> : std::true_type { };

template <typename T>
constexpr bool is_block_v = is_block<T>::value;

template <typename... Ts>
constexpr size_t block_complexity(const block_facade<Ts...>*)
{
    return ts::for_each<type_sequence<Ts...>, meta::find_max<size_t>>(
        [](auto* p, auto) {
            using item_t = std::decay_t<decltype(*p)>;

            if constexpr (is_block_v<item_t>) {
                using facade = typename facade_traits<item_t>::facade;

                return 1 + block_complexity(static_cast<facade*>(nullptr));
            }
            else
                return 0;
        });
}

} // smart_block::
