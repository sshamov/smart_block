#pragma once

#include <cstddef>
#include <utility>

namespace internal {

using std::move;
using std::nullptr_t;
using std::pair;
using std::make_pair;

template <typename> struct get_func_arg;

template <typename C, typename R, typename T>
struct get_func_arg<R(C::*)(const T&) const> { using type = T; };

template <typename F>
using get_func_arg_t = typename get_func_arg<F>::type;

template <typename List, typename Base>
struct visitor_aggregator
{
    List _funcs;

    visitor_aggregator(List&& funcs) : _funcs(move(funcs)) { }

    struct aggregated_visitor : public Base
    {
        using func_type = typename List::first_type;
        using func_arg  = get_func_arg_t<decltype(&func_type::operator())>;

        func_type _func;

        aggregated_visitor(List&& funcs) : Base(move(funcs.second)), _func(move(funcs.first)) { }

        using Base::operator();

        auto operator ()(const func_arg& val) { return _func(val); }
    };

    template <typename F>
    visitor_aggregator<pair<F, List>, aggregated_visitor> add(F&& func)
    { return { make_pair(move(func), move(_funcs)) }; }

    aggregated_visitor done() { return { move(_funcs) }; }
};

struct initial_aggregator
{
    struct null_visitor
    {
        null_visitor(nullptr_t) { }

        void operator()() { }
    };

    template <typename F>
    visitor_aggregator<std::pair<F, nullptr_t>, null_visitor> add(F&& func)
    { return { make_pair(move(func), nullptr) }; }
};

} // internal::

internal::initial_aggregator make_visitor() { return {}; }
