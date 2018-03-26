#include <stdint.h>
#include <iostream>

#include "./config.h"
#include "hash/hash_algorithm.h"
#include "helpers/uintx.h"
#include "helpers/misc.h"

#ifndef CATENA_INCLUDE_HASH_BLAMKA_H_
#define CATENA_INCLUDE_HASH_BLAMKA_H_

class BlaMka : public HashAlgorithm {
 protected:
  void *state_;
  size_t idx_;
  void *out_;

  // Gl function on which p is build
  inline void Gl(uint8_t *a, uint8_t *b, uint8_t *c, uint8_t *d);

  // permutation function
  inline void P(uint8_t *p1, uint8_t *p2, uint8_t *p3, uint8_t *p4,
                uint8_t *p5, uint8_t *p6, uint8_t *p7, uint8_t *p8);

  // Compression function of BlaMka
  inline void Compress();

 public:
  BlaMka();
  ~BlaMka();

  // loads data into buffer
  inline void Update(void *data, size_t data_len);
  inline void Final(void *out);
  void Hash(void *data, size_t data_len, void *out);
};

#endif  // CATENA_INCLUDE_HASH_BLAMKA_H_

