#include <stdint.h>
#include "./config.h"
#include "hash/hash_algorithm.h"
#include "hash/blake2b_vars.h"

#ifndef CATENA_INCLUDE_HASH_BLAKE2B_H_
#define CATENA_INCLUDE_HASH_BLAKE2B_H_

class Blake2b : public HashAlgorithm {
 protected:
    uint8_t out_len_;
    void *key_;
    uint8_t key_len_;
    uint8_t c_;
    uint64_t t_[2];
    uint64_t b_[16];
    uint64_t h_[8];

    inline uint64_t Rotr64(const uint64_t val, const uint8_t bits);

    /*
        Mixes the two words x, y into words a, b, c, d
    */
    void Mix(uint64_t *a, uint64_t *b, uint64_t *c, uint64_t *d,
             const uint64_t x, const uint64_t y);

    /*
        Compressionfunction of BLake2b
    */
    inline void Compress(bool is_last_block = false);

    /*
        Adds the value v to the member variable t_[0]
        if t_[0] overflows, t_[1] is incremented
    */
    inline void AddToT(uint8_t val);

 public:
    explicit Blake2b(uint8_t out_len, void *key = NULL_PTR,
                     uint8_t key_len = 0);
    ~Blake2b();

    void Reset();

    /*
        loads word data into buffer
        data : word
    */
    void Update(void *data, size_t data_len);
    void Final(void *out);
    void Hash(void *data, size_t data_len, void *out);
};

#endif  // CATENA_INCLUDE_HASH_BLAKE2B_H_

