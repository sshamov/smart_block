#pragma once

#include "../ext/span.h"

class hexdump
{
public:
    hexdump(const void* data, size_t length) : _data(data), _length(length) { }

    template <typename T>
    hexdump(const T& v) : _data(&v), _length(sizeof(v)) { }

    template <typename T>
    hexdump(const std::span<T>& s) : _data(as_bytes(s).data()), _length(s.size_bytes()) { }

    hexdump&  setw(size_t val) &  { _width = val; return *this; }
    hexdump&& setw(size_t val) && { return static_cast<hexdump&&>(setw(val)); }

    friend std::ostream& operator<<(std::ostream& os, const hexdump& v)
    {
        std::ostream os2(os.rdbuf());
        os2 << std::hex << std::setfill('0');

        const uint8_t* ch = reinterpret_cast<const uint8_t*>(v._data);

        std::string buf;

        size_t i;
        for (i = 0; i < v._length; i++, ch++)
        {
            if (i % v._width == 0) {
                if (i) {
                    os2 << "  " << buf << '\n';
                    buf.clear();
                }
                os2 << std::setw(4) << i << ' ';
            }

            os2 << ' ' << std::setw(2) << static_cast<unsigned>(*ch);
            buf += (0x20 <= *ch && *ch <= 0x7E) ? static_cast<char>(*ch) : '.';
        }

        os2 << std::string(3 * ((v._width - (i % v._width)) % v._width), ' ')
            << "  " << buf << '\n';

        return os;
    }

private:
    const void* _data;
    size_t      _length;
    size_t      _width = 16;
};
