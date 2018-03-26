#include <stdint.h>
#include <iostream>

#include "./config.h"

#ifndef CATENA_INCLUDE_HELPERS_XORSHIFT1024STAR_H_
#define CATENA_INCLUDE_HELPERS_XORSHIFT1024STAR_H_

namespace xorshift1024star {

class XorShift1024Star {
 public:
    XorShift1024Star();
    explicit XorShift1024Star(uint64_t *seed);
    ~XorShift1024Star();
    uint64_t Random();
    void Seed(uint64_t *seed);

 private:
    uint8_t p_;
    uint64_t *s_;
};

}  // namespace xorshift1024star
uint64_t XorShift1024Star(uint64_t *s, uint8_t p);

#endif  // CATENA_INCLUDE_HELPERS_XORSHIFT1024STAR_H_

