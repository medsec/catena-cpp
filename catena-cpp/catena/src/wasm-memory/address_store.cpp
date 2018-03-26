#include "wasm-memory/address_store.h"
#include "wasm-memory/memory.h"

namespace memory {
bool AddressStore::clear() {
  _chunk.clear();

  if (_chunk.insert(0, WASM_ADDRESS_STORE_PTR, sizeof(AddressStore))) {
    _len = 1;
    _size = sizeof(AddressStore);
    return true;
  }

  return false;
}

AddressChunk *AddressStore::find(void *ptr) {
  size_t
    len = (_len + ADDRESSCHUNK_LEN - 1) / ADDRESSCHUNK_LEN,
        lowIdx = 0,
        highIdx = len - 1,
        idx;

  AddressChunk
    *lowChunk = &_chunk,
    *highChunk = lowChunk,
    *chunk = reinterpret_cast<AddressChunk*>(NULL_PTR);

  void
    *firstPtr,
    *lastPtr;

  while (lowIdx <= highIdx) {
    idx = lowIdx + ((highIdx - lowIdx) >> 1);
    chunk = lowChunk;

    for (size_t i = lowIdx; i < idx; ++i) {
      chunk = chunk->_next;
    }

    lastPtr = chunk->_addr[chunk->_len - 1]._ptr;

    if (lastPtr < ptr) {
      lowIdx = idx + 1;
      lowChunk = chunk->_next;

      if (lowIdx == len) {
        chunk = lowChunk;
        break;
      }
    } else {
      firstPtr = chunk->_addr[0]._ptr;

      if (firstPtr <= ptr) {
        break;
      } else {
        highIdx = idx - 1;
        highChunk = lowChunk;

        for (size_t i = lowIdx; i < highIdx; ++i) {
          chunk = chunk->_next;
        }
      }
    }
  }

  return chunk;
}

size_t AddressStore::getSize() {
  return _size;
}

/*bool AddressStore::insert(void *ptr, size_t len){
  AddressChunk
 *chunk = find(ptr);

 if (chunk == WASM_NULL_PTR){
 return false;
 }

 size_t
 idx = chunk->find(ptr);

 if (idx == ADDRESSCHUNK_LEN){
 return false;
 }

 if (!chunk->insert(idx, ptr, len)){
 return false;
 }

 ++_len;

 if ((_len % ADDRESSCHUNK_LEN) == ADDRESSCHUNK_LEN - 1){
 AddressChunk
 *next = (AddressChunk*)alloc(sizeof(AddressChunk));

 if (next == WASM_NULL_PTR){
 return false;
 }

 next->clear();

 while (chunk->_next != WASM_NULL_PTR){
 chunk = chunk->_next;
 }

 chunk->_next = next;
 }

 return true;
 }*/

void *AddressStore::alloc(size_t len) {
  size_t idx = 1;

  void
    *ptr;

  AddressChunk
    *chunk = &_chunk;

  Address
    *prevAddr = &chunk->_addr[0],
    *addr;

    for (size_t i = 1; i < _len; ++i) {
      addr = &chunk->_addr[idx];

      if (prevAddr->getSpaceTo(addr) >= len) {
        break;
      }

      ++idx;
      prevAddr = addr;

      if (idx == ADDRESSCHUNK_LEN) {
        idx = 0;
        chunk = chunk->_next;
      }
    }

    ptr = reinterpret_cast<void*>((size_t)prevAddr->_ptr + prevAddr->_len);

    if (!chunk->insert(idx, ptr, len)) {
      return NULL_PTR;
    }

    ++_len;
    _size += len;

#ifdef LOG_MEMORY
    std::cout << std::dec << "mem size: " << _size << std::endl;
#endif

    if ((_len % ADDRESSCHUNK_LEN) == ADDRESSCHUNK_LEN - 1) {
      AddressChunk
        *next = reinterpret_cast<AddressChunk*>(alloc(sizeof(AddressChunk)));

      if (next == NULL_PTR) {
        return NULL_PTR;
      }

      next->clear();

      while (chunk->_next != NULL_PTR) {
        chunk = chunk->_next;
      }

      chunk->_next = next;
    }

    return ptr;
}

void *AddressStore::realloc(void *src, size_t len) {
  if (src == WASM_ADDRESS_STORE_PTR) {
    return NULL_PTR;
  }

  AddressChunk
    *chunk = find(src);

  if (chunk == NULL_PTR) {
    return NULL_PTR;
  }

  size_t
    idx = chunk->find(src);

  if (idx == ADDRESSCHUNK_LEN) {
    return NULL_PTR;
  }

  Address
    *addr = &chunk->_addr[idx];

  if (addr->_ptr != src) {
    return NULL_PTR;
  }

  void
    *ptr = alloc(len);

  if (ptr == NULL_PTR) {
    return NULL_PTR;
  }

  memory::memcpy(ptr, src, addr->_len);

  free(src);

  return ptr;
}

void AddressStore::dealloc(void *ptr) {
  if (ptr == WASM_ADDRESS_STORE_PTR) {
    return;
  }

  AddressChunk
    *chunk = find(ptr);

  if (chunk == NULL_PTR) {
    return;
  }

  size_t
    idx = chunk->find(ptr);

  if (idx == ADDRESSCHUNK_LEN) {
    return;
  }

  if (chunk->_addr[idx]._ptr == ptr) {
    size_t len = chunk->_addr[idx]._len;

    if (!chunk->remove(idx)) {
      return;
    }

    --_len;
    _size -= len;

#ifdef LOG_MEMORY
    std::cout << std::dec << "mem size: " << _size << std::endl;
#endif

    if (_len % ADDRESSCHUNK_LEN == ADDRESSCHUNK_LEN - 1) {
      AddressChunk
        *prevChunk = chunk;

      while (chunk->_next != NULL_PTR) {
        prevChunk = chunk;
        chunk = prevChunk->_next;
      }

      dealloc(prevChunk->_next);
      prevChunk->_next = reinterpret_cast<AddressChunk*>(NULL_PTR);
    }
  }
}

AddressStore *AddressStore::init() {
  AddressStore
    *store = AddressStore::get();

  store->clear();

  return store;
}

AddressStore *AddressStore::get() {
  return reinterpret_cast<AddressStore*>(WASM_ADDRESS_STORE_PTR);
}

#ifndef CATENA_PLATFORM_WASM
std::ostream &operator<<(std::ostream &stream, const AddressStore &store) {
  stream << "cnt addr " << std::hex << std::setw(sizeof(size_t) * 2) <<
    std::setfill('0') << store._len << std::endl;

  AddressChunk *chunk = reinterpret_cast<AddressChunk*>(&store._chunk);

  for (size_t i = 0; chunk != NULL_PTR; ++i) {
    stream << "chunk    " << std::hex << std::setw(2) << std::setfill('0') <<
      i << std::endl;
    stream << *chunk;

    chunk = chunk->_next;
  }

  return stream;
}
#endif
}  // namespace memory
