#pragma once

#include <cstdint>
#include <string>

void consume_ptr(void*);

template <typename T>
void consume_cref(const T&);

template <>
void consume_cref<uint32_t>(const uint32_t&);

template <>
void consume_cref<std::string>(const std::string&);
