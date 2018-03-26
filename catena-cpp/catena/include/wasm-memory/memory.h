#include <stdint.h>
#include "wasm-memory/address_store.h"

#pragma once

#ifndef MEMORY_H
#define MEMORY_H

void init();

void *operator new(size_t len);
void *operator new[](size_t len);
void operator delete(void *ptr);
void operator delete[](void *ptr);

namespace memory {
  void *malloc(size_t len);
  void *realloc(void *src, size_t len);
  void free(void *ptr);
  void *memcpy(void *dest, void *src, size_t len);
  void *memset(void *dest, uint8_t val, size_t len);
}

#endif
