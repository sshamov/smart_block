#include "consume.h"

void consume_ptr(void*) { }

template <>
void consume_cref<uint32_t>(const uint32_t&) { }

template <>
void consume_cref<std::string>(const std::string&) { }
