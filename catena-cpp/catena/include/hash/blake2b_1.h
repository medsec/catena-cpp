#include <stdint.h>
#include "./config.h"
#include "hash/hash_algorithm.h"
#include "hash/blake2b_vars.h"

#ifndef CATENA_INCLUDE_HASH_BLAKE2B_1_H_
#define CATENA_INCLUDE_HASH_BLAKE2B_1_H_

class Blake2b_1 : public HashAlgorithm {
 protected:
    uint8_t r_;
    uint8_t c_;
    uint64_t t_[2];
    uint64_t b_[16];
    uint64_t h_[8];

    inline uint64_t Rotr64(const uint64_t val, const uint8_t bits);

    /*
        Compression Function of Blake2b_1
    */
    inline void Compress();

    /*
        Adds the value v to the member variable t_[0]
        if t_[0] overflows, t_[1] is incremented
    */
    inline void AddToT(uint8_t val);

 public:
    Blake2b_1();
    ~Blake2b_1();

    void Reset();

    /*
        loads word data into buffer
        data : word
    */
    void Update(void *data, size_t data_len);

    /*
        inalises the hashing
    */
    void Final(void *out);
    void Hash(void *data, size_t data_len, void *out);
};

#endif  // CATENA_INCLUDE_HASH_BLAKE2B_1_H_

