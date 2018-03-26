#include "../../config.h"
#include "../../hash/hash_algorithm.h"
#include "../../helpers/uintx.h"
#include "./graph_algorithm.h"
#include "../../helpers/xorshift1024star.h"

#ifndef CATENA_INCLUDE_COMPONENTS_GRAPH_DRSAMPLE_GRAPH_H_
#define CATENA_INCLUDE_COMPONENTS_GRAPH_DRSAMPLE_GRAPH_H_

namespace catena {
namespace graph {
class DRSampleGraph : public GraphAlgorithm {
 public:
     DRSampleGraph(
         HashAlgorithm *h, size_t n,
         HashAlgorithm *h_prime, size_t k,
         uint8_t lambda);

     DRSampleGraph(
         HashAlgorithm *h, size_t n,
         HashAlgorithm *h_prime, size_t k,
         uint8_t lambda, xorshift1024star::XorShift1024Star *xss);

     ~DRSampleGraph();

     void Execute(uint8_t g, void *v);

 protected:
     HashAlgorithm *h_;
     HashAlgorithm *h_prime_;

     size_t n_;
     size_t k_;

     uint64_t seed_;

     uint8_t lambda_;

     xorshift1024star::XorShift1024Star *xss_;

     void *HFirst(
         void *data1, size_t data1_len,
         void *data2, size_t data2_len,
         void *out);

     uint64_t Sigma(uint64_t j);
};
}  // namespace graph
}  // namespace catena

#endif  // CATENA_INCLUDE_COMPONENTS_GRAPH_DRSAMPLE_GRAPH_H_

