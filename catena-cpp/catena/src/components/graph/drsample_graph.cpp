#include <math.h>
#include <stdlib.h>     // srand, rand
#include <iostream>
#include <algorithm>    // std::max


#include "components/graph/drsample_graph.h"
#include "helpers/misc.h"

namespace catena {
namespace graph {
DRSampleGraph::~DRSampleGraph() {
}

DRSampleGraph::DRSampleGraph(
    HashAlgorithm *h,       size_t n,
    HashAlgorithm *h_prime, size_t k,
    uint8_t lambda) {
  h_ = h;
  n_ = n;
  h_prime_ = h_prime;
  k_ = k;
  lambda_ = lambda;
}

DRSampleGraph::DRSampleGraph(
    HashAlgorithm *h,       size_t n,
    HashAlgorithm *h_prime, size_t k,
    uint8_t lambda, xorshift1024star::XorShift1024Star *xss) {
  h_ = h;
  n_ = n;
  h_prime_ = h_prime;
  k_ = k;
  lambda_ = lambda;
  xss_ = xss;
}


void *DRSampleGraph::HFirst(
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
    h_->Hash(h_out, h_out_len,
              reinterpret_cast<uint8_t*>((size_t)out + (i) * n_));
  }

  FREE(h_out);
  return out;
}


// Gets second parent for given node j, equivalent to getParent function in
// the DRSample paper
//     j: index
uint64_t DRSampleGraph::Sigma(uint64_t j) {
  uint64_t g_prime = xss_->Random() % (uint64_t) log2(j + 1);  // since j >= 0
  uint64_t parent_dist = std::min(j, (uint64_t) (1 << g_prime));
  uint64_t r =
    xss_->Random() % parent_dist + std::max(ceil(parent_dist * 0.5), 2.0);
  return j -r + 1;
  }


  void DRSampleGraph::Execute(uint8_t g, void *v) {
    const uint64_t two_to_the_g = 1 << g;

    const size_t h_prime_len = k_;

    void *r = MALLOC(h_prime_len << g);
    void *data = MALLOC(h_prime_len);

    uint8_t *v_alpha = new uint8_t[h_prime_len];
    uint8_t *v_beta = new uint8_t[h_prime_len + 1];

    MEMCPY(v_alpha, PTR_TO(v, two_to_the_g - 1, h_prime_len), h_prime_len);
    MEMCPY(v_beta, PTR_TO(v, 0, h_prime_len), h_prime_len);
    v_beta[h_prime_len] = 0;

    HFirst(v_alpha, h_prime_len,
        v_beta, h_prime_len + 1, r);


    v_beta[h_prime_len] = 1;
    HFirst(v_alpha, h_prime_len,
        v_beta, h_prime_len + 1, PTR_TO(r, 1, h_prime_len));

    for (uint64_t i = 2; i < two_to_the_g; ++i) {
          uint64_t idx = Sigma(i-1);
          h_prime_->Update(PTR_TO(r, i-1, h_prime_len), h_prime_len);
          h_prime_->Update(PTR_TO(r, idx, h_prime_len), h_prime_len);
          h_prime_->Final(PTR_TO(r, i, h_prime_len));
    }

    delete[] v_alpha;
    delete[] v_beta;
    FREE(data);
    MEMCPY(v, r, two_to_the_g * h_prime_len);
    FREE(r);
  }
}  // namespace graph
}  // namespace catena

