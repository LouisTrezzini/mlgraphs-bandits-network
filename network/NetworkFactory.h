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

        std::vector<Edge> used_by = {
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

        Network g(N, used_by);

        return g;
    }
};


#endif //MLGRAPHS_BANDITS_NETWORK_NETWORKFACTORY_H
