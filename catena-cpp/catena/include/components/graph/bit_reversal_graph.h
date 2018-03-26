#include <stdint.h>
#include "components/graph/generic_graph.h"

#ifndef CATENA_INCLUDE_COMPONENTS_GRAPH_BIT_REVERSAL_GRAPH_H_
#define CATENA_INCLUDE_COMPONENTS_GRAPH_BIT_REVERSAL_GRAPH_H_

namespace catena {
namespace graph {
class BitReversalGraph : public GenericGraph {
 protected:
    /*
        Indexing Function of Bit Reversal Graph
        val : index of which we want to compute the parents index
        bits : number of bits used (usually garlic)
        Example: parent of index 3 shall be computed, maximum index is 15:
            val=3, bits=4
            3 = 0011 --> 1100 = 12
    */
    inline uint64_t Rho(uint64_t val, uint8_t bits);
 public:
    BitReversalGraph(HashAlgorithm *h, size_t n,
                     HashAlgorithm *h_prime, size_t k,
                     uint8_t lambda);
};
}  // namespace graph
}  // namespace catena

#endif  // CATENA_INCLUDE_COMPONENTS_GRAPH_BIT_REVERSAL_GRAPH_H_

