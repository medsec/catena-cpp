#include <stdint.h>
#include "../config.h"

#ifndef CATENA_INCLUDE_HELPERS_UINTX_H_
#define CATENA_INCLUDE_HELPERS_UINTX_H_

namespace uintx {
  uint8_t *XorArray(uint8_t *left, uint8_t *right, size_t len, uint8_t *out);
  uint8_t *XorCopy(uint8_t *left, uint8_t *right, size_t len);
  uint8_t *Add(uint8_t *left, size_t len, uint8_t right);
  bool Equal(uint8_t *left, uint8_t *right, size_t len);
}

#endif  // CATENA_INCLUDE_HELPERS_UINTX_H_
