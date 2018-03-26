#include "components/gamma/saltmix.h"

namespace catena {
namespace gamma {

  SaltMix::SaltMix(HashAlgorithm *h, HashAlgorithm *h_prime) {
    h_ = h;
    h_prime_ = h_prime;
  }

  void *SaltMix::Execute(
      const uint8_t g,
      uint8_t *v,
      uint8_t *salt,
      const size_t salt_len) {
    size_t len = 1 << ((3 * (g + 1)) >> 2);
    size_t p = 0;

    const uint8_t shift_bits = 64 - g;
    const uint8_t n = 64;
    const uint8_t k = 64;

    uint64_t j1;
    uint64_t j2;

    uint8_t *v_j1;

    h_->Hash(salt, salt_len, r_);
    h_->Hash(r_, n, PTR_TO(r_, 1, n));

#ifdef BIG_ENDIAN_PLATFORM
    for (uint8_t i = 0; i < 16; ++i) {
      r_[i] = LE(r_[i]);
    }
#endif

    for (size_t i = 0; i < len; ++i) {
      // shift 64 undefined
      j1 = XorShift1024Star(r_, p) >> shift_bits;
      j2 = XorShift1024Star(r_, p + 1) >> shift_bits;

      v_j1 = reinterpret_cast<uint8_t*>(PTR_TO(v, j1, k));

      h_prime_->Update(v_j1, k);
      h_prime_->Update(PTR_TO(v, j2, k), k);
      h_prime_->Final(v_j1);

      p = (p + 2) & 0xf;
    }

    return v;
  }
}  // namespace gamma
}  // namespace catena
