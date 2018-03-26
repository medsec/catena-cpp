#include <stdint.h>

#ifndef CATENA_INCLUDE_COMPONENTS_GRAPH_GRAPH_ALGORITHM_H_
#define CATENA_INCLUDE_COMPONENTS_GRAPH_GRAPH_ALGORITHM_H_

namespace catena {
namespace graph {
class GraphAlgorithm {
 public:
    virtual void Execute(uint8_t g, void *v);
    virtual ~GraphAlgorithm() {}
};
}  // namespace graph
}  // namespace catena

#endif  // CATENA_INCLUDE_COMPONENTS_GRAPH_GRAPH_ALGORITHM_H_

