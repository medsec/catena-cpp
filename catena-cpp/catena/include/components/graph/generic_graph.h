#include "./config.h"
#include "hash/hash_algorithm.h"
#include "components/graph/graph_algorithm.h"

#ifndef CATENA_INCLUDE_COMPONENTS_GRAPH_GENERIC_GRAPH_H_
#define CATENA_INCLUDE_COMPONENTS_GRAPH_GENERIC_GRAPH_H_

namespace catena {
namespace graph {
class GenericGraph : public GraphAlgorithm {
 public:
    GenericGraph(
        HashAlgorithm *h,       size_t n,
        HashAlgorithm *h_prime, size_t k,
        uint8_t lambda);

    ~GenericGraph();

    void Execute(uint8_t g, void *v);

 protected:
    HashAlgorithm *h_;
    HashAlgorithm *h_prime_;

    size_t n_;
    size_t k_;

    uint8_t lambda_;

    // primitive indexing function, needs to be overwritten by child classes
    virtual uint64_t Rho(uint64_t val, uint8_t bits);

    // compute 2 inputs for the cryptographic weaker hashfunction H'
    //     data1: first stateword
    //     data2: second stateword
    //     out: output
    void HFirst(
        void *data1, size_t data1_len,
        void *data2, size_t data2_len,
        void *out);
};
}  // namespace graph
}  // namespace catena

#endif  // CATENA_INCLUDE_COMPONENTS_GRAPH_GENERIC_GRAPH_H_

