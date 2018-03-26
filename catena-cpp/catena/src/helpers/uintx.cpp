#include "helpers/uintx.h"

namespace uintx {
  uint8_t *XorArray(uint8_t *left, uint8_t *right, size_t len, uint8_t *out) {
    for (size_t i = 0; i < len; ++i) {
      out[i] = left[i] ^ right[i];
    }

    return out;
  }

  uint8_t *XorCopy(uint8_t *left, uint8_t *right, size_t len) {
    uint8_t *result = reinterpret_cast<uint8_t *>(MALLOC(len));

    for (size_t i = 0; i < len; ++i) {
      result[i] = left[i] ^ right[i];
    }

    return result;
  }

  uint8_t *Add(uint8_t *left, size_t len, uint8_t right) {
    uint16_t carry = right;
    size_t i = len;

    do {
      --i;
      carry += left[i];

      left[i] = carry;

      carry >>= 8;
    } while (i > 0);

    return left;
  }

  bool Equal(uint8_t *left, uint8_t *right, size_t len) {
    uint8_t result = 1;

    for (size_t i = 0; i < len; ++i) {
      if (left[i] == right[i])
        result &= 1;
      else
        result &= 0;
    }

    return result == 1;
  }
}  // namespace uintx

