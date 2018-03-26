#include "components/graph/generic_graph.h"

namespace catena {
namespace graph {
  GenericGraph::~GenericGraph() {
  }

  uint64_t GenericGraph::Rho(uint64_t val, uint8_t bits) {
    return 0;
  }

  GenericGraph::GenericGraph(
      HashAlgorithm *h, size_t n,
      HashAlgorithm *h_prime, size_t k,
      uint8_t lambda) {
    h_ = h;
    n_ = n;
    h_prime_ = h_prime;
    k_ = k;
    lambda_ = lambda;
  }

  void GenericGraph::HFirst(
      void *data1, size_t data1_len,
      void *data2, size_t data2_len,
      void *out) {
    size_t h_out_len = n_ + 1;
    uint8_t *h_out = reinterpret_cast<uint8_t*>(MALLOC(h_out_len));
    size_t len = k_ / n_;
    void *h_out_hash = PTR_TO(h_out, 1, 1);

    h_->Update(data1, data1_len);
    h_->Update(data2, data2_len);
    h_->Final(h_out_hash);

    MEMCPY(out, h_out_hash, n_);

    for (size_t i = 1; i < len; ++i) {
      h_out[0] = i;
      h_->Hash(h_out, h_out_len, out);
    }

    FREE(h_out);
  }

  void GenericGraph::Execute(uint8_t g, void *v) {
    const uint64_t two_to_the_g = 1 << g;
    const size_t h_prime_len = k_;
    void *r = MALLOC(h_prime_len << g);

    for (uint8_t k = 0; k < lambda_; ++k) {
      HFirst(PTR_TO(v, two_to_the_g - 1, h_prime_len), h_prime_len, v,
          h_prime_len, r);

      for (size_t i = 1; i < two_to_the_g; ++i) {
        h_prime_->Update(PTR_TO(r, i - 1, h_prime_len), h_prime_len);
        h_prime_->Update(PTR_TO(v, Rho(i, g), h_prime_len), h_prime_len);
        h_prime_->Final(PTR_TO(r, i, h_prime_len));
      }

      void *temp = r;
      r = v;
      v = temp;
    }

    if ((lambda_ & 0x1) == 0) {
      FREE(r);
    } else {
      MEMCPY(r, v, two_to_the_g * h_prime_len);
      FREE(v);
    }
  }
}  // namespace graph
}  // namespace catena

