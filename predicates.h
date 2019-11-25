#pragma once

#include "ext/predicate.h"
#include "meta.h"

#include <iosfwd>

namespace smart_block {

// IsVoid

constexpr auto isVoid = pred<std::is_void>();
constexpr auto isNotVoid = !isVoid;

// HasTransform

template <typename T>
struct has_transform : std::bool_constant<meta::is_complete_v<transform_item<T>>> { };

template <typename T>
constexpr auto has_transform_v = has_transform<T>::value;

constexpr auto hasTransform = pred<has_transform>();
constexpr auto hasNoTransform = !hasTransform;

using HasTransform = decltype(hasTransform);
using HasNoTransform = decltype(hasNoTransform);

// IsPlain

template <typename T>
class is_value_plain
{
    static constexpr bool get_value()
    {
        if constexpr (eval<T>(hasTransform))
            return transform_item<T>::is_plain;
        else
            return std::is_trivially_copyable_v<T>;
    }

public:
    static constexpr bool value = get_value();
};

constexpr auto isPlain = pred<is_value_plain>();
constexpr auto isNotPlain = !pred<is_value_plain>();

using IsPlain = decltype(isPlain);
using IsNotPlain = decltype(isNotPlain);

// IsIntegral

constexpr auto isIntegral = pred<std::is_integral>();
constexpr auto isNotIntegral = !isIntegral;

using IsIntegral = decltype(isIntegral);
using IsNotIntergal = decltype(isNotIntegral);

// HasRawData

template <typename T>
using has_raw_data_check = decltype(transform_item<T>::raw_data(nullptr, 0));

constexpr auto hasRawData = detector<has_raw_data_check>();
constexpr auto hasNoRawData = !hasRawData;

using HasRawData = decltype(hasRawData);
using HasNoRawData = decltype(hasNoRawData);

// HasPrint

template <typename T>
using has_print_check = decltype(transform_item<T>::print(std::declval<std::ostream&>(), nullptr, 0));

constexpr auto hasPrint = detector<has_print_check>();
constexpr auto hasNoPrint = !hasPrint;

using HasPrint = decltype(hasPrint);
using HasNoPrint = decltype(hasNoPrint);

// IsBlock

constexpr auto isBlock = pred<is_block>();
constexpr auto isNotBlock = !isBlock;

using IsBlock = decltype(isBlock);
using IsNotBlock = decltype(isNotBlock);

// IsBlockFacade

template <typename> struct is_block_cfacade : std::false_type { };

template <typename... Ts> struct is_block_cfacade<block_cfacade<Ts...>> : std::true_type { };

template <typename> struct is_block_facade : std::false_type { };

template <typename... Ts> struct is_block_facade<block_facade<Ts...>> : std::true_type { };

constexpr auto isBlockFacade = pred<is_block_cfacade>() | pred<is_block_facade>();
constexpr auto isNotBlockFacade = !isBlockFacade;

using IsBlockFacade = decltype(isBlockFacade);
using IsNotBlockFacade = decltype(isNotBlockFacade);

// IsBlockEditor

template <typename> struct is_block_builder : std::false_type { };

template <typename... Ts> struct is_block_builder<builder<Ts...>> : std::true_type { };

constexpr auto isBlockEditor = pred<is_block_facade>() | pred<is_block_builder>();
constexpr auto isNotBlockEditor = !isBlockEditor;

using IsBlockEditor = decltype(isBlockEditor);
using IsNotBlockEditor = decltype(isNotBlockEditor);

// HasSetFromCref

template <typename T, typename U>
using set_from_cref_check = decltype(std::declval<T>().set(std::declval<const U&>()));

constexpr auto hasSetFromCref = detector<set_from_cref_check>();

// IsAssignableFromCref

template <typename T, typename U>
using assignable_from_cref_check = decltype(std::declval<T>() = std::declval<const U&>());

constexpr auto isAssignableFromCref = detector<assignable_from_cref_check>();

// IsConstructible

constexpr auto isConstructible = pred<std::is_constructible>();

// IsAggregatableFromCref

template <typename T, typename U>
using aggregatable_from_cref_check = decltype(std::declval<T>() = { std::declval<const U&>() });

constexpr auto isAggregatableFromCref = detector<aggregatable_from_cref_check>();

// CanBeSetFromCref

constexpr auto canBeSetFromCref = hasSetFromCref | isAssignableFromCref | isAggregatableFromCref;
constexpr auto cannotBeSetFromCref = !canBeSetFromCref;

using CanBeSetFromCref = decltype(canBeSetFromCref);

// HasSetFromRv

template <typename T, typename U>
using set_from_rv_check = decltype(std::declval<T>().set(std::declval<U&&>()));

constexpr auto hasSetFromRv = detector<set_from_rv_check>();

using HasSetFromRv = decltype(hasSetFromRv);

// IsAssignableFromRv

template <typename T, typename U>
using assignable_from_rv_check = decltype(std::declval<T>() = std::declval<U&&>());

constexpr auto isAssignableFromRv = detector<assignable_from_rv_check>();

// CanBeSetFromCref

constexpr auto canBeSetFromRv = hasSetFromRv | isAssignableFromRv;
constexpr auto cannotBeSetFromRv = !canBeSetFromRv;

using CanBeSetFromRv = decltype(canBeSetFromRv);

} // smart_block::
