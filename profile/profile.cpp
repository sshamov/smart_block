#include "profile_impl.h"
#include "measurer.h"

#include <iostream>

using namespace std;
using namespace measurer;

template <typename Struct, typename Block>
class Profiler
{
    using unary_function = void (*)(void*);
    using binary_function = void (*)(void*, void*);

    template <typename T>
    nanoseconds::rep profile_construction(unary_function func)
    {
        array<byte, sizeof(T)> data;

        Measurer m(1000ms);
        while (!m.expired()) {
            m.start();
            func(data.data());
            m.stop();

            destruct<T>(data.data());
        }

        return m.avg();
    }

    template <typename T>
    nanoseconds::rep profile_destruction(unary_function func)
    {
        array<byte, sizeof(T)> data;

        Measurer m(1000ms);
        while (!m.expired()) {
            construct<T>(data.data());

            m.start();
            func(data.data());
            m.stop();
        }

        return m.avg();
    }

    template <typename T>
    nanoseconds::rep profile_dissection(unary_function func)
    {
        array<byte, sizeof(T)> data;

        construct<T>(data.data());

        Measurer m(1000ms);
        while (!m.expired()) {
            m.start();
            func(data.data());
            m.stop();
        }

        destruct<T>(data.data());

        return m.avg();
    }

    template <typename T>
    nanoseconds::rep profile_copying(binary_function func)
    {
        array<byte, sizeof(T)> src, dst;

        construct<T>(src.data());

        Measurer m(1000ms);
        while (!m.expired()) {
            m.start();
            func(src.data(), dst.data());
            m.stop();

            destruct<T>(dst.data());
        }

        destruct<T>(src.data());

        return m.avg();
    }

    template <typename T>
    nanoseconds::rep profile_moving(binary_function func)
    {
        array<byte, sizeof(T)> src, dst;

        Measurer m(1000ms);
        while (!m.expired()) {
            construct<T>(src.data());

            m.start();
            func(src.data(), dst.data());
            m.stop();

            destruct<T>(src.data());
            destruct<T>(dst.data());
        }

        return m.avg();
    }

public:
    void generate_report(ostream& os)
    {
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wundefined-func-template"

        /* construction */ {

        os << "construction: " << flush;
        os << setw(4) << profile_construction<Struct>(construct<Struct>) << "ns | " << flush;
        os << setw(4) << profile_construction<Block>(construct<Block>) << "ns" << flush;
        os << "   noopt: " << flush;
        os << setw(4) << profile_construction<Struct>(noopt::construct<Struct>) << "ns | " << flush;
        os << setw(4) << profile_construction<Block>(noopt::construct<Block>) << "ns" << endl;

        }

        /* destruction */ {

        os << " destruction: " << flush;
        os << setw(4) << profile_destruction<Struct>(destruct<Struct>) << "ns | " << flush;
        os << setw(4) << profile_destruction<Block>(destruct<Block>) << "ns" << flush;
        os << "   noopt: " << flush;
        os << setw(4) << profile_destruction<Struct>(noopt::destruct<Struct>) << "ns | " << flush;
        os << setw(4) << profile_destruction<Block>(noopt::destruct<Block>) << "ns" << endl;

        }

        /* dissection */ {

        os << "  dissection: " << flush;
        os << setw(4) << profile_dissection<Struct>(dissect<Struct>) << "ns | " << flush;
        os << setw(4) << profile_dissection<Block>(dissect<Block>) << "ns" << flush;
        os << "   noopt: " << flush;
        os << setw(4) << profile_dissection<Struct>(noopt::dissect<Struct>) << "ns | " << flush;
        os << setw(4) << profile_dissection<Block>(noopt::dissect<Block>) << "ns" << endl;

        }

        /* copying */ {

        os << "     copying: " << flush;
        os << setw(4) << profile_copying<Struct>(copy<Struct>) << "ns | " << flush;
        os << setw(4) << profile_copying<Block>(copy<Block>) << "ns" << flush;
        os << "   noopt: " << flush;
        os << setw(4) << profile_copying<Struct>(noopt::copy<Struct>) << "ns | " << flush;
        os << setw(4) << profile_copying<Block>(noopt::copy<Block>) << "ns" << endl;

        }

        /* moving */ {

        os << "      moving: " << flush;
        os << setw(4) << profile_moving<Struct>(move<Struct>) << "ns | " << flush;
        os << setw(4) << profile_moving<Block>(move<Block>) << "ns" << flush;
        os << "   noopt: " << flush;
        os << setw(4) << profile_moving<Struct>(noopt::move<Struct>) << "ns | " << flush;
        os << setw(4) << profile_moving<Block>(noopt::move<Block>) << "ns" << endl;

        }

        #pragma clang diagnostic pop
    }
};

using BasicSimpleProfiler = Profiler<basic_simple_struct, basic_simple_block>;
using BasicComplexProfiler = Profiler<basic_complex_struct, basic_complex_block>;
using BasicVeryComplexProfiler = Profiler<basic_very_complex_struct, basic_very_complex_block>;
using StringSimple1Profiler = Profiler<string_simple1_struct, string_simple1_block>;
using StringSimple2Profiler = Profiler<string_simple2_struct, string_simple2_block>;
using StringComplex1Profiler = Profiler<string_complex1_struct, string_complex1_block>;
using StringComplex2Profiler = Profiler<string_complex2_struct, string_complex2_block>;
using StringVeryComplex1Profiler = Profiler<string_very_complex1_struct, string_very_complex1_block>;
using StringVeryComplex2Profiler = Profiler<string_very_complex2_struct, string_very_complex2_block>;

int main()
{
    cout << "[basic simple]" << endl;
    BasicSimpleProfiler().generate_report(cout);
    cout << endl;

    cout << "[basic complex]" << endl;
    BasicComplexProfiler().generate_report(cout);
    cout << endl;

    cout << "[basic very complex]" << endl;
    BasicVeryComplexProfiler().generate_report(cout);
    cout << endl;

    cout << "[string simple #1]" << endl;
    StringSimple1Profiler().generate_report(cout);
    cout << endl;

    cout << "[string simple #2]" << endl;
    StringSimple2Profiler().generate_report(cout);
    cout << endl;

    cout << "[string complex #1]" << endl;
    StringComplex1Profiler().generate_report(cout);
    cout << endl;

    cout << "[string complex #2]" << endl;
    StringComplex2Profiler().generate_report(cout);
    cout << endl;

    cout << "[string very complex #1]" << endl;
    StringVeryComplex1Profiler().generate_report(cout);
    cout << endl;

    cout << "[string very complex #2]" << endl;
    StringVeryComplex2Profiler().generate_report(cout);
    cout << endl;

    return 0;
}
