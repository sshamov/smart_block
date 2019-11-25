#pragma once

#include "../block.h"
#include "../builder.h"

#ifdef TEST_STRUCTURED_BINDING
#include "../structured_binding.h"
#endif

namespace smart_block {

using std::byte;
using std::string;
using std::move;

// Simple

class IntPairCFacade : private block_cfacade<uint32_t, uint32_t>
{
    using self = IntPairCFacade;
    using base = block_cfacade<uint32_t, uint32_t>;

public:
    IntPairCFacade(const byte* data) : base(data) { }
    IntPairCFacade(const base& facade) : base(facade.data()) { }

    uint32_t first()  const { return get<0>(); }
    uint32_t second() const { return get<1>(); }

    FACADE_METHODS

    friend class IntPairBlock;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class IntPairFacade : private block_facade<uint32_t, uint32_t>
{
    using self = IntPairFacade;
    using base = block_facade<uint32_t, uint32_t>;

public:
    IntPairFacade(byte* data) : base(data) { }

    uint32_t first()  const { return get<0>(); }
    uint32_t second() const { return get<1>(); }

    void first(uint32_t val)  & { set<0>(val); }
    void second(uint32_t val) & { set<1>(val); }

    void first(uint32_t val)  && { first(val); }
    void second(uint32_t val) && { second(val); }

    friend class IntPairBlock;
};

class IntPairBlock : public block_storage<uint32_t, uint32_t>, public IntPairFacade
{
    using self    = IntPairBlock;
    using storage = block_storage<uint32_t, uint32_t>;
    using cfacade = IntPairCFacade;
    using facade  = IntPairFacade;

public:
    IntPairBlock() : IntPairFacade(storage::data()) { }

    IntPairBlock(const self& other) : storage(other), IntPairFacade(storage::data()) { }

    using IntPairFacade::first;
    using IntPairFacade::second;

    self&& first(uint32_t val)  && { first(val); return move(*this); }
    self&& second(uint32_t val) && { second(val); return move(*this); }

    CONSTRUCTABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

// Complex

class ComplexCFacade : private block_cfacade<uint32_t, block<uint32_t, uint32_t>, uint32_t>
{
    using self = ComplexCFacade;
    using base = block_cfacade<uint32_t, block<uint32_t, uint32_t>, uint32_t>;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, IntPairCFacade, void>;
#   endif

public:
    ComplexCFacade(const byte* data) : base(data) { }
    ComplexCFacade(const base& facade) : base(facade.data()) { }

    uint32_t first()  const { return get<0>(); }
    auto     second() const { return IntPairCFacade(sub<1>()); }
    uint32_t third()  const { return get<2>(); }

    FACADE_METHODS

    friend class ComplexBlock;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class ComplexFacade : private block_facade<uint32_t, block<uint32_t, uint32_t>, uint32_t>
{
    using self = ComplexFacade;
    using base = block_facade<uint32_t, block<uint32_t, uint32_t>, uint32_t>;

public:
    ComplexFacade(byte* data) : base(data) { }
    ComplexFacade(base&& facade) : base(facade.data()) { }

    uint32_t first()  const { return get<0>(); }
    auto     second() const { return IntPairCFacade(sub<1>()); }
    uint32_t third()  const { return get<2>(); }

    auto second() { return IntPairFacade(sub<1>().data()); }

    void first(uint32_t val)        & { set<0>(val); }
    void second(IntPairBlock&& val) & { set<1>(val); }
    void third(uint32_t val)        & { set<2>(val); }

    void first(uint32_t val)        && { set<0>(val); }
    void second(IntPairBlock&& val) && { set<1>(val); }
    void third(uint32_t val)        && { set<2>(val); }

    friend class ComplexBlock;
};

class ComplexBlock : public block_storage<uint32_t, block<uint32_t, uint32_t>, uint32_t>, public ComplexFacade
{
    using self    = ComplexBlock;
    using storage = block_storage<uint32_t, block<uint32_t, uint32_t>, uint32_t>;
    using cfacade = ComplexCFacade;
    using facade  = ComplexFacade;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, IntPairCFacade, void>;
#   endif

public:
    ComplexBlock() : ComplexFacade(storage::data()) { }

    ComplexBlock(const self& other) : storage(other), ComplexFacade(storage::data()) { }

    using ComplexFacade::first;
    using ComplexFacade::second;
    using ComplexFacade::third;

    self&& first(uint32_t val)        && { first(val); return move(*this); }
    self&& second(IntPairBlock&& val) && { set<1>(val); return move(*this); }
    self&& third(uint32_t val)        && { third(val); return move(*this); }

    CONSTRUCTABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

// VeryComplex

class VeryComplexCFacade
  : private block_cfacade<uint32_t, block<uint32_t, block<uint32_t, uint32_t>, uint32_t>, uint32_t>
{
    using self = VeryComplexCFacade;
    using base = block_cfacade<uint32_t, block<uint32_t, block<uint32_t, uint32_t>, uint32_t>, uint32_t>;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, ComplexCFacade, void>;
#   endif

public:
    VeryComplexCFacade(const byte* data) : base(data) { }

    uint32_t first()  const { return get<0>(); }
    auto     second() const { return ComplexCFacade(sub<1>()); }
    uint32_t third()  const { return get<2>(); }

    FACADE_METHODS

    friend class VeryComplexBlock;

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

class VeryComplexFacade
  : private block_facade<uint32_t, block<uint32_t, block<uint32_t, uint32_t>, uint32_t>, uint32_t>
{
    using self = VeryComplexFacade;
    using base = block_facade<uint32_t, block<uint32_t, block<uint32_t, uint32_t>, uint32_t>, uint32_t>;

public:
    VeryComplexFacade(byte* data) : base(data) { }

    uint32_t first()  const { return get<0>(); }
    auto     second() const { return ComplexCFacade(sub<1>()); }
    uint32_t third()  const { return get<2>(); }

    auto second() { return ComplexFacade(sub<1>()); }

    void first(uint32_t val)        & { set<0>(val); }
    void second(ComplexBlock&& val) & { set<1>(val); }
    void third(uint32_t val)        & { set<2>(val); }

    void first(uint32_t val)        && { set<0>(val); }
    void second(ComplexBlock&& val) && { set<1>(val); }
    void third(uint32_t val)        && { set<2>(val); }

    friend class VeryComplexBlock;
};

class VeryComplexBlock
  : public block_storage<uint32_t, block<uint32_t, block<uint32_t, uint32_t>, uint32_t>, uint32_t>,
    public VeryComplexFacade
{
    using self    = VeryComplexBlock;
    using storage = block_storage<uint32_t, block<uint32_t, block<uint32_t, uint32_t>, uint32_t>, uint32_t>;
    using cfacade = VeryComplexCFacade;
    using facade  = VeryComplexFacade;
#   ifdef TEST_STRUCTURED_BINDING
    using nested = ts::type_sequence<void, ComplexCFacade, void>;
#   endif

public:
    VeryComplexBlock() : VeryComplexFacade(storage::data()) { }

    VeryComplexBlock(const self& other) : storage(other), VeryComplexFacade(storage::data()) { }

    using VeryComplexFacade::first;
    using VeryComplexFacade::second;
    using VeryComplexFacade::third;

    self&& first(uint32_t val)        && { first(val); return move(*this); }
    self&& second(ComplexBlock&& val) && { second(move(val)); return move(*this); }
    self&& third(uint32_t val)        && { third(val); return move(*this); }

    CONSTRUCTABLE_BLOCK_METHODS

#   ifdef TEST_STRUCTURED_BINDING
    ENABLE_STRUCTURED_BINDING_INT
#   endif
};

} // smart_block::

#ifdef TEST_STRUCTURED_BINDING
ENABLE_STRUCTURED_BINDING(IntPairCFacade, IntPairBlock)
ENABLE_STRUCTURED_BINDING(ComplexCFacade, ComplexBlock)
ENABLE_STRUCTURED_BINDING(VeryComplexCFacade, VeryComplexBlock)
#endif
