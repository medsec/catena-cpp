#include <stdlib.h>
#include "wasm-memory/address.h"

#ifndef CATENA_INCLUDE_WASM_MEMORY_ADDRESS_CHUNK_H_
#define CATENA_INCLUDE_WASM_MEMORY_ADDRESS_CHUNK_H_

#define ADDRESSCHUNK_LEN 8

namespace memory {
class AddressStore;

class AddressChunk {
 protected:
    size_t _len;
    Address _addr[ADDRESSCHUNK_LEN];
    AddressChunk *_next;

    friend class AddressStore;
 public:
    void clear();
    const size_t find(void *ptr);
    bool insert(size_t idx, const Address &addr);
    bool insert(size_t idx, void *ptr, size_t len);
    bool remove(size_t idx);

#ifndef PLATFORM_WASM
    friend std::ostream &operator<<(std::ostream &stream,
                                    const AddressChunk &chunk);
    friend std::ostream &operator<<(std::ostream &stream,
                                    const AddressStore &store);
#endif
};
}  // namespace memory

#endif  // CATENA_INCLUDE_WASM_MEMORY_ADDRESS_CHUNK_H_

