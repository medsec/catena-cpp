#include "./config.h"
#include "components/gamma/gamma_algorithm.h"

#ifndef CATENA_INCLUDE_COMPONENTS_GAMMA_SALTMIX_H_
#define CATENA_INCLUDE_COMPONENTS_GAMMA_SALTMIX_H_

namespace catena {
namespace gamma {
class SaltMix : public GammaAlgorithm {
 public:
    SaltMix(HashAlgorithm *h, HashAlgorithm *hPrime);

    void *Execute(
        const uint8_t g,
        uint8_t *v,
        uint8_t *salt,
        const size_t salt_len);

 protected:
    HashAlgorithm *h_;
    HashAlgorithm *h_prime_;
    uint64_t r_[16];
};
}  // namespace gamma
}  // namespace catena

#endif  // CATENA_INCLUDE_COMPONENTS_GAMMA_SALTMIX_H_

