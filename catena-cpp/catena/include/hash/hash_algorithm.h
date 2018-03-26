#include <stdlib.h>
#include "./config.h"

#ifndef CATENA_INCLUDE_HASH_HASH_ALGORITHM_H_
#define CATENA_INCLUDE_HASH_HASH_ALGORITHM_H_

class HashAlgorithm {
 public:
    virtual void Reset();
    virtual void Update(void *data, size_t data_len);
    virtual void Final(void *out);
    virtual void Hash(void *data, size_t data_len, void *out);
    virtual ~HashAlgorithm() {}
};

#endif  // CATENA_INCLUDE_HASH_HASH_ALGORITHM_H_

