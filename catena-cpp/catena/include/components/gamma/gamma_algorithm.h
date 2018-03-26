#include "./config.h"
#include "helpers/xorshift1024star.h"
#include "hash/hash_algorithm.h"

#ifndef CATENA_INCLUDE_COMPONENTS_GAMMA_GAMMA_ALGORITHM_H_
#define CATENA_INCLUDE_COMPONENTS_GAMMA_GAMMA_ALGORITHM_H_

namespace catena {
namespace gamma {
class GammaAlgorithm {
 public:
    virtual void *Execute(
        const uint8_t g,
        uint8_t *v,
        uint8_t *salt,
        const size_t salt_len);

    virtual ~GammaAlgorithm() {}
};
}  // namespace gamma
}  // namespace catena

#endif  // CATENA_INCLUDE_COMPONENTS_GAMMA_GAMMA_ALGORITHM_H_

