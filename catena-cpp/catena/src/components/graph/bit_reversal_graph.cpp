#include "components/graph/bit_reversal_graph.h"

namespace catena {
namespace graph {

  inline uint64_t BitReversalGraph::Rho(uint64_t val, uint8_t bits) {
    val =
      ((val & 0xff) << 56) |
      ((val & 0xff00) << 40) |
      ((val & 0xff0000) << 24) |
      ((val & 0xff000000) << 8) |
      ((val & 0xff00000000) >> 8) |
      ((val & 0xff0000000000) >> 24) |
      ((val & 0xff000000000000) >> 40) |
      ((val & 0xff00000000000000) >> 56);

    val =
      ((val & 0x0f0f0f0f0f0f0f0f) << 4) |
      ((val & 0xf0f0f0f0f0f0f0f0) >> 4);

    val =
      ((val & 0x3333333333333333) << 2) |
      ((val & 0xcccccccccccccccc) >> 2);

    val =
      ((val & 0x5555555555555555) << 1) |
      ((val & 0xaaaaaaaaaaaaaaaa) >> 1);

    return val >> (64 - bits);
  }

  BitReversalGraph::BitReversalGraph(
      HashAlgorithm *h, size_t n,
      HashAlgorithm *h_prime, size_t k,
      uint8_t lambda) : GenericGraph(h, n, h_prime, k, lambda) {
  }
}  // namespace graph
}  // namespace catena
