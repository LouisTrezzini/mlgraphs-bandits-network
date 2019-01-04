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

    static Network
    createFullyConnectedStarsGraph(unsigned long numberOfStars, std::vector<unsigned long> numberOfChildren) {
        std::vector<Edge> edges;

        for (unsigned long i = 0; i < numberOfStars; i++) {
            for (unsigned long j = 0; j < i; j++) {
                edges.emplace_back(i, j);
            }
        }

        unsigned long counter = numberOfStars;
        for (unsigned long i = 0; i < numberOfStars; i++) {
            for (unsigned long j = 0; j < numberOfChildren[i]; j++) {
                edges.emplace_back(i, counter);
                counter += 1;
            }
        }

        Network g(counter, edges);

        return g;
    }


    static std::unordered_map<unsigned long, unsigned long> createStarGraphLeaders(unsigned long N) {
        std::unordered_map<unsigned long, unsigned long> leaders;

        for (unsigned long i = 1; i < N; i++) {
            leaders[i] = 0;
        }

        return leaders;
    }

    static std::unordered_map<unsigned long, unsigned long>
    createFullyConnectedStarsGraphLeaders(unsigned long numberOfStars, std::vector<unsigned long> numberOfChildren) {
        std::unordered_map<unsigned long, unsigned long> leaders;

        unsigned long counter = numberOfStars;
        for (unsigned long i = 0; i < numberOfChildren.size(); i++) {
            for (unsigned long j = 0; j < numberOfChildren[i]; j++) {
                leaders[counter] = i;
                counter += 1;
            }
        }

        return leaders;
    }

    static Network createChainStarGraph(unsigned long N, unsigned long chainLength) {
        std::vector<Edge> edges;

        unsigned long n = N - chainLength + 1;

        for (unsigned long i = 1; i < n; i++) {
            edges.emplace_back(0, i);
        }

        for (unsigned long i = n; i < N; i++) {
            edges.emplace_back(i - 1, i);
        }

        Network g(N, edges);

        return g;
    }

    static std::unordered_map<unsigned long, unsigned long>
    createChainStarGraphLeaders(unsigned long N, unsigned long chainLength) {
        std::unordered_map<unsigned long, unsigned long> leaders;

        unsigned long n = N - chainLength + 1;
        for (unsigned long i = 1; i < n; i++) {
            leaders[i] = 0;
        }

        for (unsigned long i = 0; i < N - n; i++) {
            if (i % 3 == 0) {
                leaders[n + i] = n + i + 1;
            } else if (i % 3 == 2) {
                leaders[n + i] = n + i - 1;
            }
        }


        std::cerr << "createChainStarGraphLeaders" << std::endl;
        for (auto const& pair: leaders) {
            std::cerr << "{" << pair.first << ": " << pair.second << "}" << std::endl;
        }

        return leaders;
    }
};


#endif //MLGRAPHS_BANDITS_NETWORK_NETWORKFACTORY_H
