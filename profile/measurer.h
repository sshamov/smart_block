#pragma once

#include <chrono>
#include <string>
#include <ostream>

namespace measurer {

using Clock = std::chrono::steady_clock;

using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;

class Measurer
{
    bool              _expirable;
    Clock::time_point _tmax;

    uint32_t _count;

    Clock::duration   _total;
    Clock::time_point _start;

public:
    Measurer() : _expirable(false), _count(0), _total(0) { }

    Measurer(milliseconds lifetime)
      : _expirable(true), _tmax(Clock::now() + lifetime), _count(0), _total(0) { }

    void start() { _start = Clock::now(); }

    void stop() {
        ++_count;
        _total += Clock::now() - _start;
    }

    bool expired() { return _expirable && Clock::now() >= _tmax; }

    uint32_t          count() const { return _count; }
    milliseconds::rep total() const { return duration_cast<milliseconds>(_total).count(); }
    nanoseconds::rep  avg()   const { return duration_cast<nanoseconds>(_total / _count).count(); }

    std::string count_abbr() const
    {
        if (_count % 1000000 == 0)
            return std::to_string(_count / 1000000) + 'M';
        else if (_count % 1000 == 0)
            return std::to_string(_count / 1000) + 'K';
        else
            return std::to_string(_count);
    }

    friend std::ostream& operator <<(std::ostream& os, const Measurer& val)
    {
        if (val._count > 1)
            return os << val.avg() << "ns (avg of " << val.count_abbr() << " times)";
        else
            return os << val.total() << "ms";
    }
};

template <typename T>
class MeasurablePtr
{
    T*        _ptr;
    Measurer& _measurer;

public:
    MeasurablePtr(T* ptr, Measurer& measurer) : _ptr(ptr), _measurer(measurer) { }

    ~MeasurablePtr() { _measurer.stop(); }

    T* operator ->() {
        _measurer.start();
        return _ptr;
    }
};

template <typename T>
class Measurable
{
    T&       _obj;
    Measurer _measurer;

public:
    Measurable(T& obj) : _obj(obj) { }

    MeasurablePtr<T> operator ->()
    {
        if constexpr (std::is_pointer_v<T>)
            return MeasurablePtr<T>(_obj, _measurer);
        else
            return MeasurablePtr<T>(&_obj, _measurer);
    }

    milliseconds::rep total() const { return _measurer.total(); }
    nanoseconds::rep  avg()   const { return _measurer.avg(); }

    friend std::ostream& operator <<(std::ostream& os, const Measurable& val) {
        return os << val._measurer;
    }
};

} // measurer::

#define MEASURE(measurable) \
if (bool state = false) { } else \
for (auto ptr) = measurable.operator->(); !state; state = true) \
for (auto measurable = ptr).operator->(); !state; state = true)
