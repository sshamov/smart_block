#pragma once

#include "../../builder.h"
#include "../../std/string.h"

#include "../basic_test_wrappers.h"

#ifdef TEST_STRUCTURED_BINDING
#include "../../structured_binding.h"
#endif

namespace smart_block {

using std::byte;
using std::string;
using std::move;

// StringSimple1

class StringSimple1CFacade : private block_cfacade<string, uint32_t, string>
{
    using self = StringSimple1CFacade;
    using base = block_cfacade<string, uint32_t, string>;

public:
    StringSimple1CFacade(const byte* data) : base(data) { }

    const char* first()  const { return reinterpret_cast<const char*>(raw_data<0>().data()); }
    uint32_t    second() const { return get<1>(); }
    const char* third()  const { return reinterpret_cast<const char*>(raw_data<2>().data()); }

    FACADE_METHODS

    friend class StringSimple1Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringSimple1Builder;

class StringSimple1Block : public block_storage<string, uint32_t, string>, public StringSimple1CFacade
{
    using self    = StringSimple1Block;
    using storage = block_storage<string, uint32_t, string>;
    using cfacade = StringSimple1CFacade;

public:
    using builder = StringSimple1Builder;

    StringSimple1Block(const self& other)
      : storage(other.length(), other.data()), StringSimple1CFacade(storage::data())
    { }

    StringSimple1Block(self&& other) : storage(move(other)), StringSimple1CFacade(storage::data()) { }

    StringSimple1Block(storage&& other) : storage(move(other)), StringSimple1CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringSimple1Builder : private builder<string, uint32_t, string>
{
    using self  = StringSimple1Builder;
    using base  = builder<string, uint32_t, string>;
    using block = StringSimple1Block;

public:
    void first(const string& val) & { set<0>(val); }
    void second(uint32_t val)     & { set<1>(val); }
    void third(const string& val) & { set<2>(val); }

    self&& first(const string& val) && { first(val); return move(*this); }
    self&& second(uint32_t val)     && { second(val); return move(*this); }
    self&& third(const string& val) && { third(val); return move(*this); }

    BUILDER_METHODS
};

// StringSimple2

class StringSimple2CFacade : private block_cfacade<uint32_t, string, uint32_t>
{
    using self = StringSimple2CFacade;
    using base = block_cfacade<uint32_t, string, uint32_t>;

public:
    StringSimple2CFacade(const byte* data) : base(data) { }

    uint32_t    first()  const { return get<0>(); }
    const char* second() const { return reinterpret_cast<const char*>(raw_data<1>().data()); }
    uint32_t    third()  const { return get<2>(); }

    FACADE_METHODS

    friend class StringSimple2Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringSimple2Builder;

class StringSimple2Block : public block_storage<uint32_t, string, uint32_t>, public StringSimple2CFacade
{
    using self    = StringSimple2Block;
    using storage = block_storage<uint32_t, string, uint32_t>;
    using cfacade = StringSimple2CFacade;

public:
    using builder = StringSimple2Builder;

    StringSimple2Block(const self& other)
      : storage(other.length(), other.data()), StringSimple2CFacade(storage::data())
    { }

    StringSimple2Block(self&& other) : storage(move(other)), StringSimple2CFacade(storage::data()) { }

    StringSimple2Block(storage&& other) : storage(move(other)), StringSimple2CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringSimple2Builder : private builder<uint32_t, string, uint32_t>
{
    using self  = StringSimple2Builder;
    using base  = builder<uint32_t, string, uint32_t>;
    using block = StringSimple2Block;

public:
    void first(uint32_t val)       & { set<0>(val); }
    void second(const string& val) & { set<1>(val); }
    void third(uint32_t val)       & { set<2>(val); }

    self&& first(uint32_t val)       && { first(val); return move(*this); }
    self&& second(const string& val) && { second(val); return move(*this); }
    self&& third(uint32_t val)       && { third(val); return move(*this); }

    BUILDER_METHODS
};

// StringComplex1

class StringComplex1CFacade : private block_cfacade<string, block<uint32_t, uint32_t>, string>
{
    using self = StringComplex1CFacade;
    using base = block_cfacade<string, block<uint32_t, uint32_t>, string>;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, IntPairCFacade, void>;
#   endif

public:
    StringComplex1CFacade(const byte* data) : base(data) { }
    StringComplex1CFacade(const base& facade) : base(facade.data()) { }

    const char* first()  const { return reinterpret_cast<const char*>(raw_data<0>().data()); }
    auto        second() const { return IntPairCFacade(sub<1>()); }
    const char* third()  const { return reinterpret_cast<const char*>(raw_data<2>().data()); }

    FACADE_METHODS

    friend class StringComplex1Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringComplex1Builder;

class StringComplex1Block
  : public block_storage<string, block<uint32_t, uint32_t>, string>,
    public StringComplex1CFacade
{
    using self    = StringComplex1Block;
    using storage = block_storage<string, block<uint32_t, uint32_t>, string>;
    using cfacade = StringComplex1CFacade;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, IntPairCFacade, void>;
#   endif

public:
    using builder = StringComplex1Builder;

    StringComplex1Block(const self& other)
      : storage(other.length(), other.data()), StringComplex1CFacade(storage::data())
    { }

    StringComplex1Block(self&& other) : storage(move(other)), StringComplex1CFacade(storage::data()) { }

    StringComplex1Block(storage&& other) : storage(move(other)), StringComplex1CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringComplex1Builder : private builder<string, block<uint32_t, uint32_t>, string>
{
    using self  = StringComplex1Builder;
    using base  = builder<string, block<uint32_t, uint32_t>, string>;
    using block = StringComplex1Block;

public:
    void first(const string& val)   & { set<0>(val); }
    void second(IntPairBlock&& val) & { set<1>(val); }
    void third(const string& val)   & { set<2>(val); }

    self&& first(const string& val)   && { first(val); return move(*this); }
    self&& second(IntPairBlock&& val) && { set<1>(val); return move(*this); }
    self&& third(const string& val)   && { third(val); return move(*this); }

    BUILDER_METHODS
};

// StringComplex2

class StringPairCFacade : private block_cfacade<string, string>
{
    using self = StringPairCFacade;
    using base = block_cfacade<string, string>;

public:
    StringPairCFacade(const byte* data) : base(data) { }
    StringPairCFacade(const base& facade) : base(facade.data()) { }

    const char* first()  const { return reinterpret_cast<const char*>(raw_data<0>().data()); }
    const char* second() const { return reinterpret_cast<const char*>(raw_data<1>().data()); }

    FACADE_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringPairBuilder : private builder<string, string>
{
    using self = StringPairBuilder;
    using base = builder<string, string>;
    using block = block<string, string>;

public:
    void first(const string& val)  & { set<0>(val); }
    void second(const string& val) & { set<1>(val); }

    self&& first(const string& val)  && { first(val); return move(*this); }
    self&& second(const string& val) && { second(val); return move(*this); }

    BUILDER_METHODS
};

class StringComplex2CFacade : private block_cfacade<uint32_t, block<string, string>, uint32_t>
{
    using self = StringComplex2CFacade;
    using base = block_cfacade<uint32_t, block<string, string>, uint32_t>;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, StringPairCFacade, void>;
#   endif

public:
    StringComplex2CFacade(const byte* data) : base(data) { }
    StringComplex2CFacade(const base& facade) : base(facade.data()) { }

    uint32_t first()  const { return get<0>(); }
    auto     second() const { return StringPairCFacade(sub<1>()); }
    uint32_t third()  const { return get<2>(); }

    FACADE_METHODS

    friend class StringComplex2Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringComplex2Builder;

class StringComplex2Block
  : public block_storage<uint32_t, block<string, string>, uint32_t>,
    public StringComplex2CFacade
{
    using self    = StringComplex2Block;
    using storage = block_storage<uint32_t, block<string, string>, uint32_t>;
    using cfacade = StringComplex2CFacade;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, StringPairCFacade, void>;
#   endif

public:
    using builder = StringComplex2Builder;

    StringComplex2Block(const self& other)
      : storage(other.length(), other.data()), StringComplex2CFacade(storage::data())
    { }

    StringComplex2Block(self&& other) : storage(move(other)), StringComplex2CFacade(storage::data()) { }

    StringComplex2Block(storage&& other) : storage(move(other)), StringComplex2CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringComplex2Builder : private builder<uint32_t, block<string, string>, uint32_t>
{
    using self  = StringComplex2Builder;
    using base  = builder<uint32_t, block<string, string>, uint32_t>;
    using block = StringComplex2Block;

public:
    void first(uint32_t val)             & { set<0>(val); }
    void second(StringPairBuilder&& val) & { set<1>(move(val.as_base())); }
    void third(uint32_t val)             & { set<2>(val); }

    self&& first(uint32_t val)             && { first(val); return move(*this); }
    self&& second(StringPairBuilder&& val) && { set<1>(move(val.as_base())); return move(*this); }
    self&& third(uint32_t val)             && { third(val); return move(*this); }

    BUILDER_METHODS
};

// StringVeryComplex1

class StringVeryComplex1CFacade
  : private block_cfacade<string, block<uint32_t, block<string, string>, uint32_t>, string>
{
    using self = StringVeryComplex1CFacade;
    using base = block_cfacade<string, block<uint32_t, block<string, string>, uint32_t>, string>;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, StringComplex2CFacade, void>;
#   endif

public:
    StringVeryComplex1CFacade(const byte* data) : base(data) { }

    const char* first()  const { return reinterpret_cast<const char*>(raw_data<0>().data()); }
    auto        second() const { return StringComplex2CFacade(sub<1>()); }
    const char* third()  const { return reinterpret_cast<const char*>(raw_data<2>().data()); }

    FACADE_METHODS

    friend class StringVeryComplex1Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringVeryComplex1Builder;

class StringVeryComplex1Block
  : public block_storage<string, block<uint32_t, block<string, string>, uint32_t>, string>,
    public StringVeryComplex1CFacade
{
    using self    = StringVeryComplex1Block;
    using storage = block_storage<string, block<uint32_t, block<string, string>, uint32_t>, string>;
    using cfacade = StringVeryComplex1CFacade;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, StringComplex2CFacade, void>;
#   endif

public:
    using builder = StringVeryComplex1Builder;

    StringVeryComplex1Block(const self& other)
      : storage(other.length(), other.data()), StringVeryComplex1CFacade(storage::data())
    { }

    StringVeryComplex1Block(self&& other)
      : storage(move(other)), StringVeryComplex1CFacade(storage::data())
    { }

    StringVeryComplex1Block(storage&& other)
      : storage(move(other)), StringVeryComplex1CFacade(storage::data())
    { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringVeryComplex1Builder
  : private builder<string, block<uint32_t, block<string, string>, uint32_t>, string>
{
    using self  = StringVeryComplex1Builder;
    using base  = builder<string, block<uint32_t, block<string, string>, uint32_t>, string>;
    using block = StringVeryComplex1Block;

public:
    void first(const string& val)            & { set<0>(val); }
    void second(StringComplex2Builder&& val) & { set<1>(move(val.as_base())); }
    void third(const string& val)            & { set<2>(val); }

    self&& first(const string& val)            && { first(val); return move(*this); }
    self&& second(StringComplex2Builder&& val) && { set<1>(move(val.as_base())); return move(*this); }
    self&& third(const string& val)            && { third(val); return move(*this); }

    BUILDER_METHODS
};

// StringVeryComplex2

class StringVeryComplex2CFacade
  : private block_cfacade<uint32_t, block<string, block<uint32_t, uint32_t>, string>, uint32_t>
{
    using self = StringVeryComplex2CFacade;
    using base = block_cfacade<uint32_t, block<string, block<uint32_t, uint32_t>, string>, uint32_t>;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, StringComplex1CFacade, void>;
#   endif

public:
    StringVeryComplex2CFacade(const byte* data) : base(data) { }

    uint32_t first()  const { return get<0>(); }
    auto     second() const { return StringComplex1CFacade(sub<1>()); }
    uint32_t third()  const { return get<2>(); }

    FACADE_METHODS

    friend class StringVeryComplex2Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringVeryComplex2Builder;

class StringVeryComplex2Block
  : public block_storage<uint32_t, block<string, block<uint32_t, uint32_t>, string>, uint32_t>,
    public StringVeryComplex2CFacade
{
    using self    = StringVeryComplex2Block;
    using storage = block_storage<uint32_t, block<string, block<uint32_t, uint32_t>, string>, uint32_t>;
    using cfacade = StringVeryComplex2CFacade;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, StringComplex1CFacade, void>;
#   endif

public:
    using builder = StringVeryComplex2Builder;

    StringVeryComplex2Block(const self& other)
      : storage(other.length(), other.data()), StringVeryComplex2CFacade(storage::data())
    { }

    StringVeryComplex2Block(self&& other)
      : storage(move(other)), StringVeryComplex2CFacade(storage::data())
    { }

    StringVeryComplex2Block(storage&& other)
      : storage(move(other)), StringVeryComplex2CFacade(storage::data())
    { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class StringVeryComplex2Builder
  : private builder<uint32_t, block<string, block<uint32_t, uint32_t>, string>, uint32_t>
{
    using self  = StringVeryComplex2Builder;
    using base  = builder<uint32_t, block<string, block<uint32_t, uint32_t>, string>, uint32_t>;
    using block = StringVeryComplex2Block;

public:
    void first(uint32_t val)                 & { set<0>(val); }
    void second(StringComplex1Builder&& val) & { set<1>(move(val.as_base())); }
    void third(uint32_t val)                 & { set<2>(val); }

    self&& first(uint32_t val)                 && { first(val); return move(*this); }
    self&& second(StringComplex1Builder&& val) && { set<1>(move(val.as_base())); return move(*this); }
    self&& third(uint32_t val)                 && { third(val); return move(*this); }

    BUILDER_METHODS
};

} // smart_block::

#ifdef TEST_STRUCTURED_BINDING
ENABLE_STRUCTURED_BINDING(StringSimple1CFacade, StringSimple1Block)
ENABLE_STRUCTURED_BINDING(StringSimple2CFacade, StringSimple2Block)
ENABLE_STRUCTURED_BINDING(StringComplex1CFacade, StringComplex1Block)
ENABLE_STRUCTURED_BINDING(StringPairCFacade)
ENABLE_STRUCTURED_BINDING(StringComplex2CFacade, StringComplex2Block)
ENABLE_STRUCTURED_BINDING(StringVeryComplex1CFacade, StringVeryComplex1Block)
ENABLE_STRUCTURED_BINDING(StringVeryComplex2CFacade, StringVeryComplex2Block)
#endif
