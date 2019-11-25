#pragma once

#include <cstddef>
#include <type_traits>

// predicate tree

struct PredTree { };

template <template <typename...> typename P>
struct Pred : PredTree
{
    template <typename... Ts>
    static constexpr bool eval() { return P<Ts...>::value; }
};

template <template <typename...> typename P>
constexpr Pred<P> pred() { return {}; }

// bind1st

template <template <typename...> typename Pred, typename First>
struct Bind1st : PredTree
{
    template <typename... Ts>
    static constexpr bool eval() { return Pred<First, Ts...>::value; }
};

template <template <typename...> typename Pred, typename First>
constexpr Bind1st<Pred, First> bind1st() { return {}; }

// bind2nd

template <template <typename...> typename Pred, typename Second>
struct Bind2nd : PredTree
{
    template <typename First, typename... Ts>
    static constexpr bool eval() { return Pred<First, Second, Ts...>::value; }
};

template <template <typename...> typename Pred, typename Second>
constexpr Bind2nd<Pred, Second> bind2nd() { return {}; }

// utilities

template <typename T, typename PT>
constexpr bool eval(PT) { return PT().template eval<T>(); }

template <typename T, typename PT>
using void_if = std::enable_if_t<eval<T>(PT())>;

template <typename T, typename PT, typename T1>
using type_if = std::enable_if_t<eval<T>(PT()), T1>;

template <typename T, typename PT, typename T1, typename T2>
using type_if_or = std::conditional_t<eval<T>(PT()), T1, T2>;

template <typename T, typename PT>
using Constraint = std::enable_if_t<eval<T>(PT())>;

template <typename T1, typename T2, typename PT>
using Constraint2 = std::enable_if_t<eval<T1>(PT()) && eval<T2>(PT())>;

template <size_t I>
using NeverEnabled = std::enable_if_t<false, std::integral_constant<size_t, I>>;

template <typename T, typename U, typename PT>
constexpr bool eval(PT) { return PT().template eval<T, U>(); }

template <typename T, typename U, typename PT>
using BConstraint = std::enable_if_t<eval<T, U>(PT())>;

// IsPredTree

constexpr auto isPredTree = bind1st<std::is_base_of, PredTree>();

using IsPredTree = decltype(isPredTree);

// not

template <typename P>
struct Not : PredTree
{
    template <typename... Ts>
    static constexpr bool eval()
    {
        return !P::template eval<Ts...>();
    }
};

template <typename P, typename = Constraint<P, IsPredTree>>
constexpr Not<P> operator !(P) { return {}; }

// and

template <typename... Ps>
struct And : PredTree
{
    template <typename... Ts>
    static constexpr bool eval() { return (Ps::template eval<Ts...>() & ...); }
};

template <typename P1, typename P2, typename = Constraint2<P1, P2, IsPredTree>>
constexpr And<P1, P2> operator &(P1, P2) { return {}; }

// or

template <typename... Ps>
struct Or : PredTree
{
    template <typename... Ts>
    static constexpr bool eval() { return (Ps::template eval<Ts...>() | ...); }
};

template <typename P1, typename P2, typename = Constraint2<P1, P2, IsPredTree>>
constexpr Or<P1, P2> operator |(P1, P2) { return {}; }

// xor

template <typename P1, typename P2>
struct Xor : PredTree
{
    template <typename... Ts>
    static constexpr bool eval()
    {
        return P1::template eval<Ts...>() ^
               P2::template eval<Ts...>();
    }
};

template <typename P1, typename P2, typename = Constraint2<P1, P2, IsPredTree>>
constexpr Xor<P1, P2> operator ^(P1, P2) { return {}; }

// detection idiom

template <template <typename> typename Check>
struct Detector : PredTree
{
    template <typename Void, typename>
    struct Impl : std::false_type { };

    template <typename T>
    struct Impl<std::void_t<Check<T>>, T> : std::true_type { };

    template <typename T>
    static constexpr bool eval() { return Impl<void, T>::value; }
};

template <template <typename> typename Check>
constexpr Detector<Check> detector() { return {}; }

template <template <typename, typename> typename Check>
struct Detector2 : PredTree
{
    template <typename Void, typename, typename>
    struct Impl : std::false_type { };

    template <typename T, typename U>
    struct Impl<std::void_t<Check<T, U>>, T, U> : std::true_type { };

    template <typename T, typename U>
    static constexpr bool eval() { return Impl<void, T, U>::value; }
};

template <template <typename, typename> typename Check>
constexpr Detector2<Check> detector() { return {}; }
