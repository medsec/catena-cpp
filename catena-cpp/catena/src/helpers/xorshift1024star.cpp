#include "helpers/xorshift1024star.h"

namespace xorshift1024star {

XorShift1024Star::XorShift1024Star() {
  p_ = 0;
  s_ = new uint64_t[16];
}

XorShift1024Star::XorShift1024Star(uint64_t *seed) {
  p_ = 0;
  MEMCPY(s_, seed, 128);
}

XorShift1024Star::~XorShift1024Star() {
  delete[] s_;
}

uint64_t XorShift1024Star::Random() {
  p_ = p_ & 15;
  uint64_t s0 = s_[p_];
  p_ = (p_ + 1) & 15;
  uint64_t s1 = s_[p_];
  s1 ^= (s1 << 31);
  s1 ^= (s1 >> 11);
  s0 ^= (s0 >> 30);
  s_[p_]  = (s1 ^ s0);
  uint64_t res = s_[p_] * UINT64_C(1181783497276652981);
  ++p_;
  return res;
}

void XorShift1024Star::Seed(uint64_t *seed) {
  MEMCPY(s_, seed, 128);
  p_ = 0;
}

}  // namespace xorshift1024star

uint64_t XorShift1024Star(uint64_t *s, uint8_t p) {
  const uint64_t s0 = s[p & 0xf];
  uint64_t s1 = s[p = (p + 1) & 0xf];
  s1 ^= s1 << 31;
  s[p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30);

  return s[p] * UINT64_C(1181783497276652981);
}
