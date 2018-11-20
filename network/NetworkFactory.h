#ifndef MLGRAPHS_BANDITS_NETWORK_NETWORKFACTORY_H
#define MLGRAPHS_BANDITS_NETWORK_NETWORKFACTORY_H

#include "Network.h"

class NetworkFactory {
public:
    static Network createFilesGraph() {
        enum files_e {
            dax_h,
            yow_h, boz_h, zow_h, foo_cpp,
            foo_o, bar_cpp, bar_o, libfoobar_a,
            zig_cpp, zig_o, zag_cpp, zag_o,
            libzigzag_a, killerapp,
            N
        };

        std::vector<Edge> edges = {
                Edge(dax_h, foo_cpp), Edge(dax_h, bar_cpp), Edge(dax_h, yow_h),
                Edge(yow_h, bar_cpp), Edge(yow_h, zag_cpp),
                Edge(boz_h, bar_cpp), Edge(boz_h, zig_cpp), Edge(boz_h, zag_cpp),
                Edge(zow_h, foo_cpp),
                Edge(foo_cpp, foo_o),
                Edge(foo_o, libfoobar_a),
                Edge(bar_cpp, bar_o),
                Edge(bar_o, libfoobar_a),
                Edge(libfoobar_a, libzigzag_a),
                Edge(zig_cpp, zig_o),
                Edge(zig_o, libzigzag_a),
                Edge(zag_cpp, zag_o),
                Edge(zag_o, libzigzag_a),
                Edge(libzigzag_a, killerapp)
        };

        Network g(N, edges);

        return g;
    }

    static Network createFullyConnectedGraph(unsigned long N) {
        std::vector<Edge> edges;

        for (unsigned long i = 0; i < N; i++) {
            for (unsigned long j = 0; j < i; j++) {
                edges.emplace_back(i, j);
            }
        }

        Network g(N, edges);

        return g;
    }

    static Network createFullyDisconnectedGraph(unsigned long N) {
        std::vector<Edge> edges;

        Network g(N, edges);

        return g;
    }

    static Network createCircularGraph(unsigned long N) {
        std::vector<Edge> edges;

        for (unsigned long i = 1; i < N; i++) {
            edges.emplace_back(i - 1, i);
        }

        edges.emplace_back(N - 1, 0);

        Network g(N, edges);

        return g;
    }

    static Network createStarGraph(unsigned long N) {
        std::vector<Edge> edges;

        for (unsigned long i = 1; i < N; i++) {
            edges.emplace_back(0, i);
        }

        Network g(N, edges);

        return g;
    }
};


#endif //MLGRAPHS_BANDITS_NETWORK_NETWORKFACTORY_H
