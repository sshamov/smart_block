#pragma once

#include <cstddef>
#include <ostream>
#include <iomanip>

namespace std {

inline constexpr size_t dynamic_extent = static_cast<size_t>(-1);

template <typename T, size_t Extent = dynamic_extent>
class span
{
    T*     _ptr;
    size_t _count;

public:
    using index_type = size_t;
    using pointer = T*;
    using iterator = T*;

    constexpr span(pointer ptr, index_type count) : _ptr(ptr), _count(count) { }

    constexpr pointer    data()       const noexcept { return _ptr; }
    constexpr index_type size_bytes() const noexcept { return _count * sizeof(T); }

    constexpr iterator begin() const noexcept { return _ptr; }
    constexpr iterator end()   const noexcept { return _ptr + _count; }

    friend void swap(span& lhs, span& rhs)
    {
        swap(lhs._ptr, rhs._ptr);
        swap(lhs._count, rhs._count);
    }
};

template <typename T, size_t N, size_t S = (N == dynamic_extent ? dynamic_extent : N * sizeof(T))>
span<const byte, S> as_bytes(span<T, N> s) noexcept
{
    return { reinterpret_cast<const byte*>(s.data()), s.size_bytes() };
}

template <typename T, size_t Extent>
ostream& operator <<(ostream& os, const span<T, Extent>& val)
{
    const byte*  data = as_bytes(val).data();
    const size_t size = val.size_bytes();

    os << size << (size > 1 ? " bytes <" : " byte <");

    ostream os2(os.rdbuf());

    os2 << hex << setfill('0');

    for (size_t i = 0; i < size/2; ++i) {
        if (i > 0) os2 << ' ';

        os2 << setw(2) << static_cast<int>(*data++) << '-';
        os2 << setw(2) << static_cast<int>(*data++);
    }

    if (size % 2 == 1) {
        if (size > 1) os2 << ' ';
        os2 << setw(2) << static_cast<int>(*data);
    }

    os << '>';

    return os;
}

} // std::
