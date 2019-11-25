#pragma once

namespace smart_block {

template <typename... Ts> class block_cfacade;

template <typename... Ts> class block_facade;

template <typename Facade> struct facade_traits;

template <typename... Ts> class block_storage;

template <typename... Ts> class block;

template <typename... Ts> class builder;

template <typename T, typename = void> struct transform_item;

} // smart_block::
