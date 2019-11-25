#pragma once

#include "predicates.h"

#include <ostream>

namespace smart_block {

using std::byte;
using std::ostream;

// value_length

template <typename T>
type_if<T, IsPlain, size_t> value_length(const builder_of<T>&)
{
    return sizeof(builder_of<T>);
}

template <typename T>
type_if<T, IsNotPlain, size_t> value_length(const builder_of<T>& builder)
{
    return transform_item<T>::length(builder);
}

// get_editor

template <typename T>
type_if<T, HasTransform, editor_of<T>> get_editor(builder_of<T>& builder)
{
    return transform_item<T>::get_editor(builder);
}

template <typename T>
type_if<T, HasNoTransform, T&> get_editor(T& ref)
{
    return ref;
}

// pack_value

template <typename T>
void_if<T, HasTransform> pack_value(const builder_of<T>& builder, byte* target)
{
    transform_item<T>::pack(builder, target);
}

template <typename T>
void_if<T, HasNoTransform> pack_value(const T& val, byte* target)
{
    reinterpret_cast<T&>(*target) = val;
}

// unpack_value

template <typename T>
type_if<T, HasTransform, value_of<T>> unpack_value(const byte* target, size_t length)
{
    return transform_item<T>::unpack(target, length);
}

template <typename T>
type_if<T, HasNoTransform, const T&> unpack_value(const byte* target, size_t)
{
    return reinterpret_cast<const T&>(*target);
}

// print_value

template <typename T>
void_if<T, HasTransform> print_value(ostream& os, const byte* target, size_t length)
{
    if constexpr (eval<T>(hasPrint))
        transform_item<T>::print(os, target, length);
    else
        os << transform_item<T>::unpack(target, length);
}

template <typename T>
void_if<T, HasNoTransform> print_value(ostream& os, const T& val)
{
    if constexpr (eval<T>(isIntegral))
        if (os.flags() & std::ios_base::hex) os << "0x";

    os << val;
}

template <typename T>
void_if<T, HasNoTransform> print_value(ostream& os, const byte* target, size_t)
{
    if constexpr (eval<T>(isIntegral))
        if (os.flags() & std::ios_base::hex) os << "0x";

    os << reinterpret_cast<const T&>(*target);
}

} // smart_block::
