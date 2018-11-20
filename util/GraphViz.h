#ifndef MLGRAPHS_BANDITS_NETWORK_GRAPHVIZ_H
#define MLGRAPHS_BANDITS_NETWORK_GRAPHVIZ_H

#include <iostream>
#include <boost/graph/graphviz.hpp>
#include "../network/Network.h"

namespace GraphViz {
    void write(const Network &network, const std::string &filename) {
        std::ofstream file(filename);

        if(!file.is_open()) {
            std::cout << "Unable to open file" << std::endl;
            return;
        }

        write_graphviz(file, network);
        file.close();
    }
}

#endif //MLGRAPHS_BANDITS_NETWORK_GRAPHVIZ_H
