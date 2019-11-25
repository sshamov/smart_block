#pragma once

#include <cstddef>
#include <utility>

namespace meta {

// static_for

namespace internal {

template <size_t Begin, typename Folder, typename Func, size_t... Is>
constexpr auto static_for_impl(Func&& f, std::index_sequence<Is...>)
{
    if constexpr (std::is_void_v<Folder>)
        (f(std::integral_constant<size_t, Begin + Is>()), ...);
    else {
        Folder folder;

        (folder(f(std::integral_constant<size_t, Begin + Is>())) || ...);

        return folder.result;
    }
}

} // meta::internal::

template <size_t Begin, size_t End, typename Folder = void, typename Func>
constexpr auto static_for(Func&& f)
{
    return internal::static_for_impl<Begin, Folder>(
        std::forward<Func>(f),
        std::make_index_sequence<End - Begin>());
}

// folders

template <typename T>
struct add_assign
{
    T result = 0;

    bool operator()(const T& val) { result += val; return false; }
};

template <typename T>
struct find_max
{
    T result = 0;

    bool operator()(const T& val) {
        if (result < val) result = val;
        return false;
    }
};

template <int Default = -1>
struct first_nonnegative
{
    int result = Default;

    bool operator()(int val) {
        if (val < 0) return false;
        else {
            result = val;
            return true;
        }
    }
};

// is_complete

template <typename T, typename = void>
struct is_complete : std::false_type { };

template <typename T>
struct is_complete<T, decltype(void(sizeof(T)))> : std::true_type { };

template <typename T>
static constexpr bool is_complete_v = is_complete<T>::value;

// get_optional

template <template <typename> typename Getter, typename T>
class get_optional
{
    template <typename U, typename Void>
    struct impl { using type = void; };

    template <typename U>
    struct impl<U, std::void_t<Getter<U>>> { using type = Getter<U>; };

public:
    using type = typename impl<T, void>::type;
};

template <template <typename> typename Getter, typename T>
using get_optional_t = typename get_optional<Getter, T>::type;

} // meta::
