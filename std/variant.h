#pragma once

#include "../utils.h"

#include <cstddef>
#include <cxxabi.h>
#include <ostream>
#include <variant>

namespace smart_block {

using std::byte;
using std::forward;
using std::move;
using std::variant;

template <typename... Ts> class variant_cfacade;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"

class bad_variant_access : public std::exception
{
    std::string _what;

    std::string demangle(const char* mangled)
    {
        int status = -4;

        char* demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);

        std::string result = status == 0 ? demangled : mangled;

        std::free(demangled);

        return result;
    }

public:
    bad_variant_access() : _what("bad_variant_access") { }

    template <typename T>
    bad_variant_access& wrong_type()
    {
        _what += ": doesn't hold a value of '";
        _what += demangle(typeid(T).name());
        _what += "'";

        return *this;
    }

    const char* what() const noexcept { return _what.c_str(); }
};

#pragma clang diagnostic push

namespace internal {

// have_same_length

template <typename... Ts>
constexpr bool have_same_length(type_sequence<Ts...>)
{
    using seq = type_sequence<Ts...>;

    if constexpr (ts::size<seq> == 1)
        return true;
    else if constexpr (ts::size<seq> == 2)
        return sizeof(builder_of<ts::get_t<0, seq>>) ==
               sizeof(builder_of<ts::get_t<1, seq>>);
    else
    {
        using first_two = ts::trunc_t<2, seq>;
        using except_first = ts::shift_t<1, seq>;

        if constexpr (have_same_length(except_first()))
            return have_same_length(first_two());
        else
            return false;
    }
}

// is_variant_plain

template <typename... Ts>
constexpr bool is_variant_plain(type_sequence<Ts...>)
{
    if constexpr (isAllPlainable<Ts...>)
        return have_same_length(type_sequence<Ts...>());
    else
        return false;
}

// lambda_visitor

template <typename Func>
class lambda_visitor
{
    Func _func;

public:
    lambda_visitor(Func&& func) : _func(move(func)) { }

    template <typename T>
    auto operator ()(const T& val) { return _func(val); }
};

struct raw_visitor_tag { };

template <typename Func>
class lambda_raw_data_visitor : raw_visitor_tag
{
    Func _func;

public:
    lambda_raw_data_visitor(Func&& func) : _func(move(func)) { }

    template <typename T>
    auto operator ()(T*, const byte* target, size_t length)
    { return _func(static_cast<T*>(nullptr), target, length); }
};

} // smart_block::internal::

// predicates

template <typename... Ts>
constexpr bool isVariantPlain = internal::is_variant_plain(type_sequence<Ts...>());

constexpr auto isRawVisitor = bind1st<std::is_base_of, internal::raw_visitor_tag>();
constexpr auto isNotRawVisitor = !isRawVisitor;

using IsRawVisitor = decltype(isRawVisitor);
using IsNotRawVisitor = decltype(isNotRawVisitor);

constexpr auto isMonostate = bind1st<std::is_same, std::monostate>();
constexpr auto isNotMonostate =  !isMonostate;

using IsMonostate = decltype(isMonostate);
using IsNotMonostate = decltype(isNotMonostate);

namespace internal {

// visit_raw_data

template <typename T, typename Visitor, typename = Constraint<Visitor, IsRawVisitor>>
auto visit_raw_data(Visitor&& visitor, const byte* target, size_t length, raw_visitor_tag* = nullptr)
{
    return visitor(static_cast<T*>(nullptr), target, length);
}

template <typename T, typename Visitor, typename = Constraint<Visitor, IsNotRawVisitor>>
auto visit_raw_data(Visitor&& visitor, const byte* target, size_t length)
{
    if constexpr (eval<T>(hasTransform))
        return visitor(transform_item<T>::unpack(target, length));
    else
        return visitor(reinterpret_cast<const T&>(*target));
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"

// visit_plain

template <size_t Count, typename Visitor, typename Variant, typename Seq>
auto visit_plain_impl(Visitor&& visitor, const Variant& variant, Seq seq)
{
    if (variant.index() == Count) {
        using type   = ts::get_t<Count, Seq>;
        using holded = builder_of<type>;

        const holded& val = std::get<holded>(variant);

        if constexpr (eval<type>(hasTransform))
            return visitor(transform_item<type>::unpack(reinterpret_cast<const byte*>(&val), sizeof(type)));
        else
            return visitor(val);
    }
    else if constexpr (Count > 0)
        return visit_plain_impl<Count-1>(forward<Visitor>(visitor), variant, seq);
}

template <typename Visitor, typename Builder, typename Seq>
auto visit_plain(Visitor&& visitor, const Builder& builder, Seq seq)
{
    return visit_plain_impl<ts::size<Seq>-1>(forward<Visitor>(visitor), builder, seq);
}

// visit_not_plain

template <size_t Count, typename Visitor, typename Seq>
auto visit_not_plain_impl(Visitor&& visitor, const byte* data, size_t length, Seq)
{
    if (static_cast<size_t>(*data) == Count) {
        using type = ts::get_t<Count, Seq>;

        return visit_raw_data<type>(forward<Visitor>(visitor), data+1, length-1);
    }
    else if constexpr (Count > 0)
        return visit_not_plain_impl<Count-1>(forward<Visitor>(visitor), data, length, Seq());
}

template <typename Visitor, typename Seq>
auto visit_not_plain(Visitor&& visitor, const byte* data, size_t length, Seq)
{
    if constexpr (eval<ts::get_t<0, Seq>>(isMonostate)) {
        if (length == 0) {
            if constexpr (eval<Visitor>(isRawVisitor))
                return visitor(static_cast<std::monostate*>(nullptr), nullptr, 0);
            else {
                static std::monostate dummy;
                return visitor(dummy);
            }
        }
    }

    return visit_not_plain_impl<ts::size<Seq>-1>(forward<Visitor>(visitor), data, length, Seq());
}

#pragma clang diagnostic pop

// variant_cfacade implementations

template <bool Plain, typename... Ts> class variant_cfacade_impl;

// plain

template <typename... Ts>
class variant_cfacade_impl<true, Ts...>
{
    template <typename T>
    using unpacked_value = type_if_or<T, HasTransform, value_of<T>, const T&>;

    const byte* _data;

    const variant<builder_of<Ts>...>& as_variant() const
    { return reinterpret_cast<const variant<builder_of<Ts>...>&>(*_data); }

public:
    variant_cfacade_impl(const byte* data, size_t) : _data(data) { }

    size_t index() const { return as_variant().index(); }

    template <typename T>
    unpacked_value<T> as() const
    {
        if (std::holds_alternative<builder_of<T>>(as_variant())) {
            const byte* target = reinterpret_cast<const byte*>(&std::get<builder_of<T>>(as_variant()));

            return unpack_value<T>(target, sizeof(builder_of<T>));
        }
        else
            throw bad_variant_access().wrong_type<T>();
    }

    template <typename Visitor>
    auto apply(Visitor&& visitor)
    {
        return visit_plain(forward<Visitor>(visitor), as_variant(), type_sequence<Ts...>());
    }

    friend ostream& operator <<(ostream& os, const variant_cfacade_impl& val)
    {
        visit_plain(internal::lambda_visitor(
            [&os](const auto& v) { print_value(os, v); }),
            val.as_variant(), type_sequence<Ts...>());

        return os;
    }
};

// not plain

template <typename... Ts>
class variant_cfacade_impl<false, Ts...>
{
    template <typename T>
    using unpacked_value = type_if_or<T, HasTransform, value_of<T>, const T&>;

    const byte* _data = nullptr;
    size_t      _length = 0;

public:
    variant_cfacade_impl(const byte* data, size_t length) : _data(data), _length(length) { }

    size_t index() const
    {
        if (_length > 0)
            return static_cast<size_t>(*_data);
        else
            return 0;
    }

    template <typename T>
    unpacked_value<T> as() const
    {
        bool is_hold = visit_not_plain(internal::lambda_raw_data_visitor(
            [](auto* p, const byte*, size_t)
            { return std::is_same_v<T, std::decay_t<decltype(*p)>>; }),
            _data, _length, type_sequence<Ts...>());

        if (is_hold)
            return unpack_value<T>(_data+1, _length-1);
        else
            throw bad_variant_access().wrong_type<T>();
    }

    template <typename Visitor>
    auto apply(Visitor&& visitor)
    {
        return visit_not_plain(forward<Visitor>(visitor), _data, _length, type_sequence<Ts...>());
    }

    friend std::ostream& operator <<(ostream& os, const variant_cfacade_impl& val)
    {
        if (val._length == 0) {
            os << "<null>";
            return os;
        }

        visit_not_plain(internal::lambda_raw_data_visitor(
            [&os](auto* p, const byte* target, size_t length) {
                using type = std::decay_t<decltype(*p)>;

                if constexpr (eval<type>(isNotMonostate))
                    print_value<type>(os, target, length);
            }),
            val._data, val._length, type_sequence<Ts...>());

        return os;
    }
};

} // smart_block::internal::

// variant_cfacade

template <typename... Ts>
class variant_cfacade : public internal::variant_cfacade_impl<isVariantPlain<Ts...>, Ts...>
{
    using impl = internal::variant_cfacade_impl<isVariantPlain<Ts...>, Ts...>;

public:
    variant_cfacade(const byte* data, size_t length) : impl(data, length) { }
};

// variant_facade

template <typename... Ts>
class variant_facade
{
    variant<builder_of<Ts>...>& _builder;

public:
    variant_facade(variant<builder_of<Ts>...>& builder) : _builder(builder) { }

    template <typename T>
    editor_of<T> as()
    {
        if (!std::holds_alternative<builder_of<T>>(_builder))
            _builder.template emplace<builder_of<T>>();

        return get_editor<T>(std::get<builder_of<T>>(_builder));
    }
};

// transform_item

template <typename... Ts>
struct transform_item<variant<Ts...>>
{
    static constexpr bool is_plain = isVariantPlain<Ts...>;

    using value_type = variant_cfacade<Ts...>;
    using builder    = variant<builder_of<Ts>...>;
    using editor     = variant_facade<Ts...>;

    static editor get_editor(builder& builder) { return { builder }; }

    using types = type_sequence<Ts...>;
    using first = ts::get_t<0, types>;

    static size_t length(const builder& builder)
    {
        if (eval<first>(isMonostate) && builder.index() == 0) return 0;

        return 1 + ts::for_each<types, meta::first_nonnegative<-2>>(
            [&](auto* p, auto) {
                using type   = std::decay_t<decltype(*p)>;
                using holded = builder_of<type>;

                if (std::holds_alternative<holded>(builder))
                    return static_cast<int>(value_length<type>(std::get<holded>(builder)));
                else
                    return -1;
            }
        );
    }

    static void pack(const builder& builder, byte* target)
    {
        if (eval<first>(isMonostate) && builder.index() == 0) return;

        *target++ = static_cast<byte>(builder.index());

        ts::for_each<types, meta::first_nonnegative<>>(
            [&](auto* p, auto) {
                using type   = std::decay_t<decltype(*p)>;
                using holded = builder_of<type>;

                if (std::holds_alternative<holded>(builder)) {
                    pack_value<type>(std::get<holded>(builder), target);
                    return 0;
                }
                else
                    return -1;
            }
        );
    }

    static value_type unpack(const byte* target, size_t length) { return { target, length }; }
};

} // smart_block::
