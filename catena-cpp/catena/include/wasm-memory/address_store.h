#include <stdlib.h>
#include "wasm-memory/address_chunk.h"

#ifndef CATENA_INCLUDE_WASM_MEMORY_ADDRESS_STORE_H_
#define CATENA_INCLUDE_WASM_MEMORY_ADDRESS_STORE_H_

namespace memory {
class AddressStore {
 protected:
    size_t _size;
    size_t _len;

    AddressChunk _chunk;
 public:
    bool clear();
    AddressChunk *find(void *ptr);
    // bool insert(void *ptr, size_t len);
    void *alloc(size_t len);
    void *realloc(void *src, size_t len);
    void dealloc(void *ptr);

    size_t getSize();

    static AddressStore *init();
    static AddressStore *get();

#ifndef PLATFORM_WASM
    friend std::ostream &operator<<(std::ostream &stream,
                                    const AddressStore &store);
#endif
};
}  // namespace memory

#endif  // CATENA_INCLUDE_WASM_MEMORY_ADDRESS_STORE_H_

