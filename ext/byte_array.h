#pragma once

#include "span.h"

#include <array>

template <size_t N>
class byte_array : private std::array<std::byte, N>
{
    using base = std::array<std::byte, N>;

public:
    constexpr byte_array(const int(&list)[N]) {
        for (auto [in, out] = std::pair(list, base::data()); in != list + N; ++in, ++out)
            *out = static_cast<std::byte>(*in);
    }

    const std::byte* data() const { return base::data(); }

    size_t size() const { return base::size(); }

    std::span<const std::byte> span() const { return { base::data(), base::size() }; }

    bool operator ==(const std::span<const std::byte>& other) const  {
        return base::size() == other.size_bytes() &&
               memcmp(base::data(), other.data(), base::size()) == 0;
    }

    bool operator ==(const std::span<std::byte>& other) const  {
        return base::size() == other.size_bytes() &&
               memcmp(base::data(), other.data(), base::size()) == 0;
    }

    friend std::ostream& operator <<(std::ostream& os, const byte_array& val)
    { return os << val.span(); }
};
