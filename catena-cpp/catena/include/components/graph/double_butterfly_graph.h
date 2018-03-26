#include "./config.h"
#include "hash/hash_algorithm.h"
#include "helpers/uintx.h"
#include "components/graph/graph_algorithm.h"

#ifndef CATENA_INCLUDE_COMPONENTS_GRAPH_DOUBLE_BUTTERFLY_GRAPH_H_
#define CATENA_INCLUDE_COMPONENTS_GRAPH_DOUBLE_BUTTERFLY_GRAPH_H_

namespace catena {
namespace graph {
class DoubleButterflyGraph : public GraphAlgorithm {
 public:
    DoubleButterflyGraph(
        HashAlgorithm *h, size_t n,
        HashAlgorithm *h_prime, size_t k,
        uint8_t lambda);

    ~DoubleButterflyGraph();

    void Execute(uint8_t g, void *v);

 protected:
    HashAlgorithm *h_;
    HashAlgorithm *h_prime_;

    size_t n_;
    size_t k_;

    uint8_t _lambda;

    // Computes 2 (concatenated) inputs for H'
    //     data1: first state word
    //     data2: second state word
    //     out: output
    void HFirst(
        void *data1, size_t data1_len,
        void *data2, size_t data2_len,
        void *out);

    // indexing function of the Double Butterfly Graph
    //     g: garlic
    //     j: layer in the Double Butterfly Graph
    //     i: current index
    uint64_t Sigma(uint8_t g, uint16_t j, uint64_t r);
};
}  // namespace graph
}  // namespace catena

#endif  // CATENA_INCLUDE_COMPONENTS_GRAPH_DOUBLE_BUTTERFLY_GRAPH_H_

