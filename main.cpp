#include <iostream>
#include <cstdlib>
#include "network/Network.h"
#include "network/Edge.h"
#include "bandit/Bandit.h"
#include "bandit/BanditNetwork.h"
#include "bandit/ArmBernoulli.h"
#include "policy/UCBNetworkPolicy.h"


int main(int argc, char *argv[]) {

    int mode = 0;
    if (argc >= 2) mode = atoi(argv[1]);
    switch (mode) {

        case 0: {
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

            std::cout << N << " " << g.vertex_set() << std::endl;

            std::vector<IArm*> arms = {
                    new ArmBernoulli(0.5),
                    new ArmBernoulli(0.9),
            };
            const Bandit MAB(arms);
            const BanditNetwork banditNetwork(&MAB, &g);

            UCBNetworkPolicy policy(&banditNetwork);

            std::default_random_engine generator;
            policy.run(generator, 10000);

            return 0;
        }

        default:
            return 0;
    }
}
