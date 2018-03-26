#include <stdlib.h>
#include "../config.h"

#ifndef PLATFORM_WASM
#include <iostream>
#include <iomanip>
#endif

#pragma once

#ifndef ADDRESS_H
#define ADDRESS_H

namespace memory {
class AddressChunk;
class AddressStore;

class Address {
 protected:
    void *_ptr;
    size_t _len;

    friend class AddressChunk;
    friend class AddressStore;
 public:
    void clear();
    size_t getSpaceTo(Address *addr);
    Address &operator =(Address const &addr);

#ifndef PLATFORM_WASM
    friend std::ostream &operator<<(std::ostream& stream,
                                    const Address &addr);
#endif
};
}  // namespace memory

#endif
