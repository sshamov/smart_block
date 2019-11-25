#pragma once

#include "../block.h"

namespace block = smart_block;

template <typename... Ts>
std::string to_string(const block::block<Ts...>& block)
{
    return static_cast<std::stringstream&&>(std::stringstream() << std::hex << block).str();
}

template <typename... Ts>
std::string to_string(const block::block_storage<Ts...>& storage)
{
    block::block_cfacade<Ts...> facade(storage.data());

    return static_cast<std::stringstream&&>(std::stringstream() << std::hex << facade).str();
}

template <typename... Ts>
std::string to_string2(const block::block<Ts...>& block)
{
    using block::topdown;

    return static_cast<std::stringstream&&>(std::stringstream() << topdown(1) << std::hex << block).str();
}

template <typename... Ts>
std::string to_string2(const block::block_storage<Ts...>& storage)
{
    using block::topdown;

    block::block_cfacade<Ts...> facade(storage.data());

    return static_cast<std::stringstream&&>(std::stringstream() << topdown(1) << std::hex << facade).str();
}
