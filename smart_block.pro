TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH = .

DEFINES += TEST_STRUCTURED_BINDING

SOURCES = \
    test/basic_test.cpp \
    test/std/array_test.cpp \
    test/std/optional_test.cpp \
    test/std/string_test.cpp \
    test/std/variant_test.cpp \
    profile/consume.cpp \
    profile/profile.cpp \
    profile/profile_impl.cpp \
    profile/profile_impl_block.cxx \
    profile/profile_impl_noopt.cpp \
    profile/profile_impl_struct.cxx

HEADERS = \
    block.h \
    facade.h \
    fwd.h \
    meta.h \
    predicates.h \
    utils.h \
    builder.h \
    structured_binding.h \
    std/array.h \
    std/string.h \
    std/optional.h \
    std/variant.h \
    ext/byte_array.h \
    ext/meta.h \
    ext/predicate.h \
    ext/span.h \
    ext/type_sequence.h \
    test/std/array_test_data.h \
    test/std/string_test_data.h \
    test/std/string_test_wrappers.h \
    test/std/optional_test_data.h \
    test/std/optional_test_wrappers.h \
    test/std/variant_test_data.h \
    test/aggregated_visitor.h \
    test/basic_test_data.h \
    test/basic_test_wrappers.h \
    test/hexdump.h \
    test/test_utils.h \
    profile/consume.h \
    profile/measurer.h \
    profile/profile_impl.h
