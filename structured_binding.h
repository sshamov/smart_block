#pragma once
#define STRUCTURED_BINDING_H

#include "ext/meta.h"
#include "predicates.h"

namespace smart_block { namespace internal {

template <typename Block>
using get_nested = typename Block::nested;

template <typename Block>
using nested_or_void = meta::get_optional_t<get_nested, Block>;

}} // smart_block::internal::

namespace std {

namespace block = smart_block;
namespace ts = type_sequence;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmismatched-tags"

template <typename T> struct tuple_size;

template <size_t I, typename T> struct tuple_element;

template <typename... Ts>
struct tuple_size<block::block_cfacade<Ts...>>
  : integral_constant<size_t, block::block_cfacade<Ts...>::size()>::type
{
};

template <typename... Ts>
struct tuple_size<block::block<Ts...>> : tuple_size<block::block_cfacade<Ts...>> { };

template <size_t Index, typename... Ts>
struct tuple_element<Index, block::block_cfacade<Ts...>>
{
    using cfacade = block::block_cfacade<Ts...>;
    using item_t  = ts::get_t<Index, typename block::facade_traits<cfacade>::types>;

    using type = std::conditional_t<
        eval<item_t>(block::hasTransform),
        block::value_of<item_t>,
        const item_t&
    >;
};

template <size_t Index, typename... Ts>
struct tuple_element<Index, block::block<Ts...>> : tuple_element<Index, block::block_cfacade<Ts...>> { };

#pragma clang diagnostic pop

} // std::

// SUPRESS/RESTORE_WARNING

#define PRAGMA(STR) _Pragma(#STR)

#define SUPRESS_WARNING(EXPR)          \
PRAGMA(clang diagnostic push)          \
PRAGMA(clang diagnostic ignored #EXPR) \

#define RESTORE_WARNING PRAGMA(clang diagnostic pop)

// ENABLE_STRUCTURED_BINDING

#define ENABLE_STRUCTURED_BINDING_INT                                        \
private:                                                                     \
friend class meta::get_optional<internal::get_nested, self>;                 \
friend struct std::tuple_size<self>;                                         \
template <size_t, typename> friend struct std::tuple_element;                \
                                                                             \
template <size_t Index>                                                      \
using type = ts::get_t<Index, facade_traits<base>::types>;                   \
                                                                             \
using _nested = std::conditional_t<                                          \
    eval<internal::nested_or_void<self>>(isNotVoid),                         \
    internal::nested_or_void<self>,                                          \
    ts::repeat_t<size(), void>                                               \
>;                                                                           \
                                                                             \
template <size_t Index>                                                      \
using nested_facade = std::conditional_t<                                    \
    eval<ts::get_t<Index, _nested>>(isNotVoid),                              \
    ts::get_t<Index, _nested>,                                               \
    value_of<type<Index>>                                                    \
>;                                                                           \
                                                                             \
public:                                                                      \
template <size_t Index, typename = Constraint<type<Index>, HasTransform>>    \
nested_facade<Index> get() const { return { base::get<Index>() }; }          \
                                                                             \
template <size_t Index, typename = Constraint<type<Index>, HasNoTransform>>  \
const type<Index>& get() const { return base::get<Index>(); }

#define ENABLE_STRUCTURED_BINDING_1(CLASS)                         \
SUPRESS_WARNING(-Wmismatched-tags)                                 \
namespace std {                                                    \
                                                                   \
template <> struct tuple_size<block::CLASS>                        \
  : integral_constant<size_t, block::CLASS::size()>                \
{                                                                  \
};                                                                 \
                                                                   \
template <size_t Index> struct tuple_element<Index, block::CLASS>  \
{                                                                  \
    using item_t = block::CLASS::type<Index>;                      \
                                                                   \
    using type = conditional_t<                                    \
        eval<item_t>(block::hasTransform),                         \
        block::CLASS::nested_facade<Index>,                        \
        const item_t&                                              \
    >;                                                             \
};                                                                 \
                                                                   \
} /* std:: */                                                      \
RESTORE_WARNING

#define ENABLE_STRUCTURED_BINDING_2(CLASS1, CLASS2)                 \
                                                                    \
ENABLE_STRUCTURED_BINDING_1(CLASS1)                                 \
                                                                    \
SUPRESS_WARNING(-Wmismatched-tags)                                  \
namespace std {                                                     \
                                                                    \
template <> struct tuple_size<block::CLASS2>                        \
  : tuple_size<block::CLASS1> { };                                  \
                                                                    \
template <size_t Index> struct tuple_element<Index, block::CLASS2>  \
  : tuple_element<Index, block::CLASS1> { };                        \
                                                                    \
} /* std:: */                                                       \
RESTORE_WARNING

#define SELECT_MACRO(_1, _2, NAME, ...) NAME

#define ENABLE_STRUCTURED_BINDING(...) SELECT_MACRO(__VA_ARGS__,  \
    ENABLE_STRUCTURED_BINDING_2,                                  \
    ENABLE_STRUCTURED_BINDING_1                                   \
)(__VA_ARGS__)
