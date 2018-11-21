#ifndef MLGRAPHS_BANDITS_NETWORK_NETWORK_H
#define MLGRAPHS_BANDITS_NETWORK_NETWORK_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "Edge.h"

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;

class Network : public Graph {

public:
    Network() = default;

    explicit Network(unsigned long N, const std::vector<Edge> &edges)
            : Graph(edges.cbegin(), edges.cend(), N) {
    }

    auto getVertices() const {
        return make_iterator_range(boost::vertices(*this));
    }

    auto getNeighbors(unsigned long node) const {
        return make_iterator_range(boost::adjacent_vertices(node, *this));
    }

};

#endif //MLGRAPHS_BANDITS_NETWORK_NETWORK_H
