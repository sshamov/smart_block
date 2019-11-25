#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace type_sequence {

// type_sequence

template <typename... Ts>
struct type_sequence {};

// index_sequence_for

template <typename... Ts>
struct index_sequence_for;

template <typename... Ts>
struct index_sequence_for<type_sequence<Ts...>> : std::index_sequence_for<Ts...> { };

// is_type_sequence

template <typename T>
struct is_type_sequence { static constexpr bool value = false; };

template <typename... Ts>
struct is_type_sequence<type_sequence<Ts...>> { static constexpr bool value = true; };

template <typename T>
using IsTypeSequence = std::enable_if_t<is_type_sequence<T>::value>;

// size

template <typename T, typename = IsTypeSequence<T>>
constexpr size_t size = 0;

template <typename... Ts>
constexpr size_t size<type_sequence<Ts...>> = sizeof...(Ts);

// length

template <typename T, typename = IsTypeSequence<T>>
constexpr size_t length = 0;

template <typename... Ts>
constexpr size_t length<type_sequence<Ts...>> = (sizeof(Ts) + ... + 0);

// get

template <size_t Pos, typename Seq>
struct get;

template <size_t Pos, typename Seq>
using get_t = typename get<Pos, Seq>::type;

template <size_t Pos, typename Head, typename... Tail>
struct get<Pos, type_sequence<Head, Tail...>>
{
    using type = get_t<
        Pos-1,
        type_sequence<Tail...>
    >;
};

template <typename Head, typename... Tail>
struct get<0, type_sequence<Head, Tail...>>
{
    using type = Head;
};

// index_of

template <typename T, typename Seq>
struct index_of;

template <typename T, typename Seq>
constexpr int index_of_v = index_of<T, Seq>::value;

template <typename T, typename Head, typename... Tail>
struct index_of<T, type_sequence<Head, Tail...>>
{
    static constexpr size_t value = 1 + index_of_v<T, type_sequence<Tail...>>;
};

template <typename T, typename... Tail>
struct index_of<T, type_sequence<T, Tail...>>
{
    static constexpr size_t value = 0;
};

// prepend

template <typename T, typename Seq>
struct prepend;

template <typename T, typename Seq>
using prepend_t = typename prepend<T, Seq>::type;

template <typename T, typename... Ts>
struct prepend<T, type_sequence<Ts...>>
{
    using type = type_sequence<T, Ts...>;
};

// append

template <typename T, typename Seq>
struct append;

template <typename T, typename Seq>
using append_t = typename append<T, Seq>::type;

template <typename T, typename... Ts>
struct append<T, type_sequence<Ts...>>
{
    using type = type_sequence<Ts..., T>;
};

// repeat

template <size_t Count, typename T>
struct repeat
{
    using type = append_t<T, typename repeat<Count-1, T>::type>;
};

template <typename T>
struct repeat<1, T>
{
    using type = type_sequence<T>;
};

template <size_t Count, typename T>
using repeat_t = typename repeat<Count, T>::type;

// trunc

template <size_t Count, typename Seq>
struct trunc;

template <size_t Count, typename Seq>
using trunc_t = typename trunc<Count, Seq>::type;

template <size_t Count, typename Head, typename... Tail>
struct trunc<Count, type_sequence<Head, Tail...>>
{
    using type = prepend_t<
        Head,
        trunc_t<Count-1, type_sequence<Tail...>>
    >;
};

template <typename Head, typename... Tail>
struct trunc<0, type_sequence<Head, Tail...>>
{
    using type = type_sequence<>;
};

template <>
struct trunc<0, type_sequence<>>
{
    using type = type_sequence<>;
};

// shift

template <size_t Count, typename Seq>
struct shift;

template <size_t Count, typename Seq>
using shift_t = typename shift<Count, Seq>::type;

template <size_t Count, typename Head, typename... Tail>
struct shift<Count, type_sequence<Head, Tail...>>
{
    using type = shift_t<Count-1, type_sequence<Tail...>>;
};

template <typename Head, typename... Tail>
struct shift<0, type_sequence<Head, Tail...>>
{
    using type = type_sequence<Head, Tail...>;
};

template <>
struct shift<0, type_sequence<>>
{
    using type = type_sequence<>;
};

// split

template <typename Seq, typename Pred>
class split;

template <typename Seq, typename Pred>
using split_f = typename split<Seq, Pred>::first;

template <typename Seq, typename Pred>
using split_s = typename split<Seq, Pred>::second;

template <typename Head, typename... Tail, typename Pred>
class split<type_sequence<Head, Tail...>, Pred>
{
    using tail_t = split<type_sequence<Tail...>, Pred>;
    using tail_f = typename tail_t::first;
    using tail_s = typename tail_t::second;

public:
    using first = std::conditional_t<
        Pred::template eval<Head>(),
        prepend_t<Head, tail_f>,
        tail_f
    >;

    using second = std::conditional_t<
        Pred::template eval<Head>(),
        tail_s,
        prepend_t<Head, tail_s>
    >;
};

template <typename Pred>
class split<type_sequence<>, Pred>
{
public:
    using first = type_sequence<>;
    using second = type_sequence<>;
};

// concat

template <typename Seq1, typename Seq2>
struct concat;

template <typename Seq1, typename Seq2>
using concat_t = typename concat<Seq1, Seq2>::type;

template <typename... Ts1, typename... Ts2>
struct concat<type_sequence<Ts1...>, type_sequence<Ts2...>>
{
    using type = type_sequence<Ts1..., Ts2...>;
};

// partition

template <typename Seq, typename Pred>
class partition;

template <typename Seq, typename Pred>
using partition_t = typename partition<Seq, Pred>::type;

template <typename Seq, typename Pred>
constexpr size_t partition_v = partition<Seq, Pred>::value;

template <typename Head, typename... Tail, typename Pred>
class partition<type_sequence<Head, Tail...>, Pred>
{
    using tail = partition<type_sequence<Tail...>, Pred>;
    using tail_t = typename tail::type;

public:
    using type = std::conditional_t<
        Pred::template eval<Head>(),
        prepend_t<Head, tail_t>,
        append_t<Head, tail_t>
    >;

    static constexpr size_t value =
        (Pred::template eval<Head>() ? 1 : 0) + tail::value;
};

template <typename Pred>
class partition<type_sequence<>, Pred>
{
public:
    using type = type_sequence<>;

    static constexpr size_t value = 0;
};

// partition_stable

template <typename Seq, typename Pred>
class stable_partition;

template <typename Seq, typename Pred>
using stable_partition_t = typename stable_partition<Seq, Pred>::type;

template <typename Seq, typename Pred>
constexpr size_t stable_partition_v = stable_partition<Seq, Pred>::value;

template <typename Seq, typename Pred>
class stable_partition
{
    using first_t = split_f<Seq, Pred>;
    using second_t = split_s<Seq, Pred>;

public:
    using type = concat_t<first_t, second_t>;

    static constexpr size_t value = size<first_t>;
};

// filter

template <typename Seq, typename Pred>
class filter;

template <typename Seq, typename Pred>
using filter_t = typename filter<Seq, Pred>::type;

template <typename Head, typename... Tail, typename Pred>
class filter<type_sequence<Head, Tail...>, Pred>
{
    using tail_t = filter_t<type_sequence<Tail...>, Pred>;

public:
    using type = std::conditional_t<
        Pred::template eval<Head>(),
        prepend_t<Head, tail_t>,
        tail_t
    >;
};

template <typename Pred>
class filter<type_sequence<>, Pred>
{
public:
    using type = type_sequence<>;
};

// count

template <typename Seq, typename Pred>
struct count;

template <typename Seq, typename Pred>
constexpr size_t count_v = count<Seq, Pred>::value;

template <typename Head, typename... Tail, typename Pred>
struct count<type_sequence<Head, Tail...>, Pred>
{
    static constexpr size_t value =
        (Pred::template eval<Head>() ? 1 : 0) + count_v<type_sequence<Tail...>, Pred>;
};

template <typename Pred>
struct count<type_sequence<>, Pred>
{
    static constexpr size_t value = 0;
};

// for_each

namespace internal {

template <typename Folder, typename Seq, typename Func, size_t... Is>
constexpr auto for_each_impl(Seq, Func&& f, std::index_sequence<Is...>)
{
    if constexpr (std::is_void_v<Folder>)
        (f(static_cast<get_t<Is, Seq>*>(nullptr), std::integral_constant<size_t, Is>()), ...);
    else {
        Folder folder;

        (folder(f(static_cast<get_t<Is, Seq>*>(nullptr), std::integral_constant<size_t, Is>())) || ...);

        return folder.result;
    }
}

} // type_sequence::internal::

template <typename Seq, typename Folder = void, typename Func>
constexpr auto for_each(Func&& f)
{
    return internal::for_each_impl<Folder>(Seq(), std::forward<Func>(f), index_sequence_for<Seq>());
}

} // type_sequence::

using type_sequence::type_sequence;
