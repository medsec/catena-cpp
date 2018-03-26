#include "components/graph/double_butterfly_graph.h"

namespace catena {
namespace graph {
  DoubleButterflyGraph::~DoubleButterflyGraph() {
  }

  DoubleButterflyGraph::DoubleButterflyGraph(
      HashAlgorithm *h, size_t n,
      HashAlgorithm *h_prime, size_t k,
      uint8_t lambda) {
    h_ = h;
    n_ = n;
    h_prime_ = h_prime;
    k_ = k;
    _lambda = lambda;
  }

  void DoubleButterflyGraph::HFirst(
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

  uint64_t DoubleButterflyGraph::Sigma(uint8_t g, uint16_t j, uint64_t i) {
    if (j < g) {
      return (i ^ (1 << (g - 1 - j)));
    } else {
      return (i ^ (1 << (j - g + 1)));
    }
  }

  void DoubleButterflyGraph::Execute(uint8_t g, void *v) {
    const uint64_t two_to_the_g = 1 << g;
    const uint64_t two_g_minus_one = (g << 1) - 1;

    const size_t h_prime_len = k_;

    void *r = MALLOC(h_prime_len << g);
    void *temp;
    void *data = MALLOC(h_prime_len);

    for (uint8_t k = 0; k < _lambda; ++k) {
      for (uint16_t j = 0; j < two_g_minus_one; ++j) {
        uintx::XorArray(
            reinterpret_cast<uint8_t*>(PTR_TO(v, two_to_the_g - 1,
                h_prime_len)),
            reinterpret_cast<uint8_t*>(v),
            h_prime_len,
            reinterpret_cast<uint8_t*>(data));

        HFirst(
            data, h_prime_len,
            PTR_TO(v, Sigma(g, j, 0), h_prime_len), h_prime_len,
            r);

        for (uint64_t i = 1; i < two_to_the_g; ++i) {
          uintx::XorArray(
              reinterpret_cast<uint8_t*>(PTR_TO(r, i - 1, h_prime_len)),
              reinterpret_cast<uint8_t*>(PTR_TO(v, i, h_prime_len)),
              h_prime_len,
              reinterpret_cast<uint8_t*>(data));

          h_prime_->Update(data, h_prime_len);
          h_prime_->Update(PTR_TO(v, Sigma(g, j, i), h_prime_len),
              h_prime_len);
          h_prime_->Final(PTR_TO(r, i, h_prime_len));
        }

        temp = v;
        v = r;
        r = temp;
      }
    }

    FREE(data);

    if ((_lambda & 0x1) == 0) {
      FREE(r);
    } else {
      MEMCPY(r, v, two_to_the_g * h_prime_len);
      FREE(v);
    }
  }
}  // namespace graph
}  // namespace catena
