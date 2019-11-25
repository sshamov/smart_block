#include "profile_impl.h"
#include "consume.h"

template <typename T>
void destruct(void* obj) { static_cast<T*>(obj)->~T(); }

template <typename T>
void copy(void* src, void* dst) { new (dst) T(*static_cast<T*>(src)); }

template <typename T>
void move(void* src, void* dst) { new (dst) T(std::move(*static_cast<T*>(src))); }

#define SKIP_SHARED
#include "profile_impl_struct.cxx"
#include "profile_impl_block.cxx"
