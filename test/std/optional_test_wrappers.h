#pragma once

#include "../../block.h"
#include "../../std/optional.h"
#include "../../std/string.h"

#include "../basic_test_wrappers.h"
#include "string_test_wrappers.h"

#ifdef TEST_STRUCTURED_BINDING
#include "../../structured_binding.h"
#endif

namespace smart_block {

using std::byte;
using std::string;
using std::move;

// OptionalSimple1

class OptionalSimple1CFacade : private block_cfacade<uint32_t, std::optional<uint32_t>, uint32_t>
{
    using self = OptionalSimple1CFacade;
    using base = block_cfacade<uint32_t, std::optional<uint32_t>, uint32_t>;

public:
    OptionalSimple1CFacade(const byte* data) : base(data) { }

    uint32_t           first()  const { return get<0>(); }
    optional<uint32_t> second() const { return get<1>(); }
    uint32_t           third()  const { return get<2>(); }

    FACADE_METHODS

    friend class OptionalSimple1Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalSimple1Builder;

class OptionalSimple1Block
  : public block_storage<uint32_t, std::optional<uint32_t>, uint32_t>,
    public OptionalSimple1CFacade
{
    using self    = OptionalSimple1Block;
    using storage = block_storage<uint32_t, std::optional<uint32_t>, uint32_t>;
    using cfacade = OptionalSimple1CFacade;

public:
    using builder = OptionalSimple1Builder;

    OptionalSimple1Block(const self& other)
      : storage(other.length(), other.data()), OptionalSimple1CFacade(storage::data())
    { }

    OptionalSimple1Block(self&& other) : storage(move(other)), OptionalSimple1CFacade(storage::data()) { }

    OptionalSimple1Block(storage&& other) : storage(move(other)), OptionalSimple1CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalSimple1Builder : private builder<uint32_t, std::optional<uint32_t>, uint32_t>
{
    using self  = OptionalSimple1Builder;
    using base  = builder<uint32_t, std::optional<uint32_t>, uint32_t>;
    using block = OptionalSimple1Block;

public:
    void first(uint32_t val)  & { set<0>(val); }
    void second(uint32_t val) & { set<1>(val); }
    void third(uint32_t val)  & { set<2>(val); }

    self&& first(uint32_t val)  && { first(val); return move(*this); }
    self&& second(uint32_t val) && { second(val); return move(*this); }
    self&& third(uint32_t val)  && { third(val); return move(*this); }

    BUILDER_METHODS
};

// OptionalSimple2

class OptionalSimple2CFacade : private block_cfacade<string, optional<uint32_t>, string>
{
    using self = OptionalSimple2CFacade;
    using base = block_cfacade<string, optional<uint32_t>, string>;

public:
    OptionalSimple2CFacade(const byte* data) : base(data) { }

    const char*        first()  const { return reinterpret_cast<const char*>(raw_data<0>().data()); }
    optional<uint32_t> second() const { return get<1>(); }
    const char*        third()  const { return reinterpret_cast<const char*>(raw_data<2>().data()); }

    FACADE_METHODS

    friend class OptionalSimple2Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalSimple2Builder;

class OptionalSimple2Block
  : public block_storage<string, optional<uint32_t>, string>,
    public OptionalSimple2CFacade
{
    using self    = OptionalSimple2Block;
    using storage = block_storage<string, optional<uint32_t>, string>;
    using cfacade = OptionalSimple2CFacade;

public:
    using builder = OptionalSimple2Builder;

    OptionalSimple2Block(const self& other)
      : storage(other.length(), other.data()), OptionalSimple2CFacade(storage::data())
    { }

    OptionalSimple2Block(self&& other) : storage(move(other)), OptionalSimple2CFacade(storage::data()) { }

    OptionalSimple2Block(storage&& other) : storage(move(other)), OptionalSimple2CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalSimple2Builder : private builder<string, optional<uint32_t>, string>
{
    using self  = OptionalSimple2Builder;
    using base  = builder<string, optional<uint32_t>, string>;
    using block = OptionalSimple2Block;

public:
    void first(const string& val)  & { set<0>(val); }
    void second(uint32_t val)      & { set<1>(val); }
    void third(const string& val)  & { set<2>(val); }

    self&& first(const string& val)  && { first(val); return move(*this); }
    self&& second(uint32_t val)      && { second(val); return move(*this); }
    self&& third(const string& val)  && { third(val); return move(*this); }

    BUILDER_METHODS
};

// OptionalSimple3

class OptionalSimple3CFacade : private block_cfacade<uint32_t, optional<string>, uint32_t>
{
    using self = OptionalSimple3CFacade;
    using base = block_cfacade<uint32_t, optional<string>, uint32_t>;

public:
    OptionalSimple3CFacade(const byte* data) : base(data) { }

    uint32_t         first()  const { return get<0>(); }
    optional<string> second() const { return get<1>(); }
    uint32_t         third()  const { return get<2>(); }

    FACADE_METHODS

    friend class OptionalSimple3Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalSimple3Builder;

class OptionalSimple3Block
  : public block_storage<uint32_t, optional<string>, uint32_t>,
    public OptionalSimple3CFacade
{
    using self    = OptionalSimple3Block;
    using storage = block_storage<uint32_t, optional<string>, uint32_t>;
    using cfacade = OptionalSimple3CFacade;

public:
    using builder = OptionalSimple3Builder;

    OptionalSimple3Block(const self& other)
      : storage(other.length(), other.data()), OptionalSimple3CFacade(storage::data())
    { }

    OptionalSimple3Block(self&& other) : storage(move(other)), OptionalSimple3CFacade(storage::data()) { }

    OptionalSimple3Block(storage&& other) : storage(move(other)), OptionalSimple3CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalSimple3Builder : private builder<uint32_t, optional<string>, uint32_t>
{
    using self  = OptionalSimple3Builder;
    using base  = builder<uint32_t, optional<string>, uint32_t>;
    using block = OptionalSimple3Block;

public:
    void first(uint32_t val)       & { set<0>(val); }
    void second(const string& val) & { set<1>(val); }
    void third(uint32_t val)       & { set<2>(val); }

    self&& first(uint32_t val)       && { first(val); return move(*this); }
    self&& second(const string& val) && { second(val); return move(*this); }
    self&& third(uint32_t val)       && { third(val); return move(*this); }

    BUILDER_METHODS
};

// OptionalComplex1

class OptionalComplex1CFacade : private block_cfacade<uint32_t, optional<block<uint32_t, uint32_t>>, uint32_t>
{
    using self = OptionalComplex1CFacade;
    using base = block_cfacade<uint32_t, optional<block<uint32_t, uint32_t>>, uint32_t>;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, optional<IntPairCFacade>, void>;
#   endif

public:
    OptionalComplex1CFacade(const byte* data) : base(data) { }

    bool has_second() const { return get<1>().has_value(); }

    uint32_t first()  const { return get<0>(); }
    auto     second() const { return IntPairCFacade(get<1>().value()); }
    uint32_t third()  const { return get<2>(); }

    FACADE_METHODS

    friend class OptionalComplex1Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalComplex1Builder;

class OptionalComplex1Block
  : public block_storage<uint32_t, optional<block<uint32_t, uint32_t>>, uint32_t>,
    public OptionalComplex1CFacade
{
    using self    = OptionalComplex1Block;
    using storage = block_storage<uint32_t, optional<block<uint32_t, uint32_t>>, uint32_t>;
    using cfacade = OptionalComplex1CFacade;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, optional<IntPairCFacade>, void>;
#   endif

public:
    using builder = OptionalComplex1Builder;

    OptionalComplex1Block(const self& other)
      : storage(other.length(), other.data()), OptionalComplex1CFacade(storage::data())
    { }

    OptionalComplex1Block(self&& other) : storage(move(other)), OptionalComplex1CFacade(storage::data()) { }

    OptionalComplex1Block(storage&& other) : storage(move(other)), OptionalComplex1CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalComplex1Builder : private builder<uint32_t, optional<block<uint32_t, uint32_t>>, uint32_t>
{
    using self  = OptionalComplex1Builder;
    using base  = builder<uint32_t, optional<block<uint32_t, uint32_t>>, uint32_t>;
    using block = OptionalComplex1Block;

public:
    void first(uint32_t val)        & { set<0>(val); }
    void second(IntPairBlock&& val) & { set<1>(val); }
    void third(uint32_t val)        & { set<2>(val); }

    self&& first(uint32_t val)        && { first(val); return move(*this); }
    self&& second(IntPairBlock&& val) && { set<1>(val); return move(*this); }
    self&& third(uint32_t val)        && { third(val); return move(*this); }

    BUILDER_METHODS
};

// OptionalComplex2

class OptionalComplex2CFacade : private block_cfacade<string, optional<block<uint32_t, uint32_t>>, string>
{
    using self = OptionalComplex2CFacade;
    using base = block_cfacade<string, optional<block<uint32_t, uint32_t>>, string>;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, optional<IntPairCFacade>, void>;
#   endif

public:
    OptionalComplex2CFacade(const byte* data) : base(data) { }

    bool has_second() const { return get<1>().has_value(); }

    const char* first()  const { return reinterpret_cast<const char*>(raw_data<0>().data()); }
    auto        second() const { return IntPairCFacade(get<1>().value()); }
    const char* third()  const { return reinterpret_cast<const char*>(raw_data<2>().data()); }

    FACADE_METHODS

    friend class OptionalComplex2Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalComplex2Builder;

class OptionalComplex2Block
  : public block_storage<string, optional<block<uint32_t, uint32_t>>, string>,
    public OptionalComplex2CFacade
{
    using self    = OptionalComplex2Block;
    using storage = block_storage<string, optional<block<uint32_t, uint32_t>>, string>;
    using cfacade = OptionalComplex2CFacade;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, optional<IntPairCFacade>, void>;
#   endif

public:
    using builder = OptionalComplex2Builder;

    OptionalComplex2Block(const self& other)
      : storage(other.length(), other.data()), OptionalComplex2CFacade(storage::data())
    { }

    OptionalComplex2Block(self&& other) : storage(move(other)), OptionalComplex2CFacade(storage::data()) { }

    OptionalComplex2Block(storage&& other) : storage(move(other)), OptionalComplex2CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalComplex2Builder : private builder<string, optional<block<uint32_t, uint32_t>>, string>
{
    using self  = OptionalComplex2Builder;
    using base  = builder<string, optional<block<uint32_t, uint32_t>>, string>;
    using block = OptionalComplex2Block;

public:
    void first(const string& val)   & { set<0>(val); }
    void second(IntPairBlock&& val) & { set<1>(val); }
    void third(const string& val)   & { set<2>(val); }

    self&& first(const string& val)   && { first(val); return move(*this); }
    self&& second(IntPairBlock&& val) && { set<1>(val); return move(*this); }
    self&& third(const string& val)   && { third(val); return move(*this); }

    BUILDER_METHODS
};

// OptionalComplex3

class OptionalComplex3CFacade : private block_cfacade<uint32_t, optional<block<string, string>>, uint32_t>
{
    using self = OptionalComplex3CFacade;
    using base = block_cfacade<uint32_t, optional<block<string, string>>, uint32_t>;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, optional<StringPairCFacade>, void>;
#   endif

public:
    OptionalComplex3CFacade(const byte* data) : base(data) { }

    bool has_second() const { return get<1>().has_value(); }

    uint32_t first()  const { return get<0>(); }
    auto     second() const { return StringPairCFacade(get<1>().value()); }
    uint32_t third()  const { return get<2>(); }

    FACADE_METHODS

    friend class OptionalComplex3Block;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalComplex3Builder;

class OptionalComplex3Block
  : public block_storage<uint32_t, optional<block<string, string>>, uint32_t>,
    public OptionalComplex3CFacade
{
    using self    = OptionalComplex3Block;
    using storage = block_storage<uint32_t, optional<block<string, string>>, uint32_t>;
    using cfacade = OptionalComplex3CFacade;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, optional<StringPairCFacade>, void>;
#   endif

public:
    using builder = OptionalComplex3Builder;

    OptionalComplex3Block(const self& other)
      : storage(other.length(), other.data()), OptionalComplex3CFacade(storage::data())
    { }

    OptionalComplex3Block(self&& other) : storage(move(other)), OptionalComplex3CFacade(storage::data()) { }

    OptionalComplex3Block(storage&& other) : storage(move(other)), OptionalComplex3CFacade(storage::data()) { }

    BUILDABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class OptionalComplex3Builder : private builder<uint32_t, optional<block<string, string>>, uint32_t>
{
    using self  = OptionalComplex3Builder;
    using base  = builder<uint32_t, optional<block<string, string>>, uint32_t>;
    using block = OptionalComplex3Block;

public:
    void first(uint32_t val)             & { set<0>(val); }
    void second(StringPairBuilder&& val) & { set<1>(move(val.as_base())); }
    void third(uint32_t val)             & { set<2>(val); }

    self&& first(uint32_t val)             && { first(val); return move(*this); }
    self&& second(StringPairBuilder&& val) && { set<1>(move(val.as_base())); return move(*this); }
    self&& third(uint32_t val)             && { third(val); return move(*this); }

    BUILDER_METHODS
};

} // smart_block::

#ifdef TEST_STRUCTURED_BINDING
ENABLE_STRUCTURED_BINDING(OptionalSimple1CFacade, OptionalSimple1Block)
ENABLE_STRUCTURED_BINDING(OptionalSimple2CFacade, OptionalSimple2Block)
ENABLE_STRUCTURED_BINDING(OptionalSimple3CFacade, OptionalSimple3Block)
ENABLE_STRUCTURED_BINDING(OptionalComplex1CFacade, OptionalComplex1Block)
ENABLE_STRUCTURED_BINDING(OptionalComplex2CFacade, OptionalComplex2Block)
ENABLE_STRUCTURED_BINDING(OptionalComplex3CFacade, OptionalComplex3Block)
#endif
